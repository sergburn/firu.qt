#include <e32math.h>
#include <d32dbms.h>

#include "firudbschema.h"
#include "firudata.h"

const TInt KMinGoodRate = 3;

TInt64 gRandSeed;

// ----------------------------------------------------------

void ResetAndDestroyDataEntryArray( TAny* aPtr )
{
    RPointerArray<CFiruEntry>* array = reinterpret_cast<RPointerArray<CFiruEntry>*> ( aPtr );
    array->ResetAndDestroy();
}

// ----------------------------------------------------------
// ----------------------------------------------------------

CFiruEntry* CFiruEntry::NewLC( TInt aId, const TDesC& aText )
{
    CFiruEntry* self = new ( ELeave ) CFiruEntry;
    CleanupStack::PushL( self );
    self->iId = aId;
    self->iText = aText.AllocL();
    return self;
}

// ----------------------------------------------------------

CFiruEntry::CFiruEntry()
{
}

// ----------------------------------------------------------

CFiruEntry::~CFiruEntry()
{
    delete iText;
    iTranslations.ResetAndDestroy();
}

// ----------------------------------------------------------

TUint CFiruEntry::NumTranslations() const
{
    return iTranslations.Count();
}

// ----------------------------------------------------------

TPtrC CFiruEntry::TranslationText( TUint aIndex ) const
{
    if ( aIndex < iTranslations.Count() )
        return iTranslations[aIndex]->Entry().Text();
    else
        return TPtrC();
}

// ----------------------------------------------------------

void CFiruEntry::AddTranslationL( CFiruTranslation* aTranslation )
{
    iTranslations.AppendL( aTranslation );
}

// ----------------------------------------------------------
// ----------------------------------------------------------

CFiruData* CFiruData::NewL( RFs& aFs )
{
    CFiruData* self = new ( ELeave ) CFiruData( aFs );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
}

// ----------------------------------------------------------

CFiruData::CFiruData( RFs& aFs ) :
    iFs( aFs )
{
    iSourceLang = ELangFinnish;
    iTargetLang = ELangRussian;
}

// ----------------------------------------------------------

CFiruData::~CFiruData()
{
    delete iTableNameSources;
    delete iTableNameTranslations;
    delete iTableNameTargets;
    iTableTranslations.Close();
    iTableSources.Close();
    iTableTargets.Close();
    iTableSourcesById.Close();
    iTableTargetsById.Close();
    iDb.Compact();
    iDb.Close();
}

// ----------------------------------------------------------

void CFiruData::ConstructL()
{
    OpenDatabaseL();
    TTime now;
    now.UniversalTime();
    gRandSeed = now.Int64();
}

// ----------------------------------------------------------

void CFiruData::OpenDatabaseL()
{
    TInt err = iDb.Open( iFs, KDatabaseFile );
    if ( err == KErrNotFound )
    {
        User::LeaveIfError( iDb.Create( iFs, KDatabaseFile ) );
    }
    if ( iDb.IsDamaged() )
    {
        User::LeaveIfError( iDb.Recover() );
    }
}

// ----------------------------------------------------------

void CFiruData::SelectDictionaryL( TLanguage aInputLanguage, TLanguage aOutputLanguage )
{
    iTableSources.Close();
    iTableTargets.Close();
    iTableTranslations.Close();

    FiruDbSchema::CreateDictionaryL( iDb, aInputLanguage, aOutputLanguage );

    iSourceLang = aInputLanguage;
    iTargetLang = aOutputLanguage;
    iReversed = EFalse;

    iTableNameSources = FiruDbSchema::EntriesTableNameLC( aInputLanguage );
    CleanupStack::Pop( iTableNameSources );
    iTableSources.Open( iDb, *iTableNameSources, RDbTable::EUpdatable );
    iTableSourcesById.Open( iDb, *iTableNameSources, RDbTable::EUpdatable );

    iTableNameTargets = FiruDbSchema::EntriesTableNameLC( aOutputLanguage );
    CleanupStack::Pop( iTableNameTargets );
    iTableTargets.Open( iDb, *iTableNameTargets, RDbTable::EUpdatable );
    iTableTargetsById.Open( iDb, *iTableNameTargets, RDbTable::EUpdatable );

    iTableNameTranslations
        = FiruDbSchema::TranslationsTableNameLC( aInputLanguage, aOutputLanguage );
    CleanupStack::Pop( iTableNameTranslations );
    iTableTranslations.Open( iDb, *iTableNameTranslations, RDbTable::EUpdatable );

    DumpL();

    SetTableIndexL();
}

// ----------------------------------------------------------

void CFiruData::EvaluateViewL( RDbView& aView, const TDesC& aQuery )
{
    TInt err = aView.Prepare( iDb, aQuery );
//    RDebug::Print( _L("Prepare query '%S', error %d"), &aQuery, err );
    User::LeaveIfError( err );
    err = aView.EvaluateAll();
//    RDebug::Print( _L("Evaluate query '%S', error %d, count %d"), &aQuery, err, aView.CountL() );
    User::LeaveIfError( err );
}

// ----------------------------------------------------------

void CFiruData::ReverseDictionaryL()
{
    iReversed = !iReversed;
    SetTableIndexL();
}

// ----------------------------------------------------------

void CFiruData::SetTableIndexL()
{
    User::LeaveIfError( iTableSources.SetIndex( KTextIndexName ) );
    User::LeaveIfError( iTableTargets.SetIndex( KTextIndexName ) );
    User::LeaveIfError( iTableSourcesById.SetIndex( KPrimaryKeyIndexName ) );
    User::LeaveIfError( iTableTargetsById.SetIndex( KPrimaryKeyIndexName ) );
    if ( !iReversed )
        User::LeaveIfError( iTableTranslations.SetIndex( KSourceFkIndexName ) );
    else
        User::LeaveIfError( iTableTranslations.SetIndex( KTargetFkIndexName ) );
}

// ----------------------------------------------------------

void CFiruData::GetLanguagesL( TLanguage& aInputLanguage, TLanguage& aOutputLanguage ) const
{
    aInputLanguage = iSourceLang;
    aOutputLanguage = iTargetLang;
}

// ----------------------------------------------------------

TInt CFiruData::AddEntryL( const TDesC& aEntry )
{
    return AddEntryL( aEntry, iReversed );
}

// ----------------------------------------------------------

TInt CFiruData::AddEntryL( const TDesC& aEntry, TBool aReversed )
{
    TUint32 id = 0;
    RDbTable* table = aReversed ? &iTableTargets : &iTableSources;

    // avoid duplicate entries
    if ( table->SeekL( aEntry ) )
    {
        table->GetL();
        id = table->ColUint32( KColumnId );
    }
    else
    {
        id = FiruDbSchema::AddEntryL( *table, aEntry );
    }
    return id;
}

// ----------------------------------------------------------

TInt CFiruData::AddTranslationL( TInt aEntryId, const TDesC& aTranslation )
{
    TUint32 targetId = AddEntryL( aTranslation, !iReversed );

    // avoid duplicate links
    RDbView view;
    CleanupClosePushL( view );
    if ( !FindTranslationRowL( view, aEntryId, targetId ) )
    {
        if ( !iReversed )
            FiruDbSchema::AddTranslationL( iTableTranslations, aEntryId, targetId );
        else
            FiruDbSchema::AddTranslationL( iTableTranslations, targetId, aEntryId );
    }
    CleanupStack::PopAndDestroy( &view );
    return targetId;
}

// ----------------------------------------------------------

TBool CFiruData::FindTranslationRowL( RDbView& aView, TInt aEntryId, TInt aTargetId )
{
    TBuf<128> query;
    _LIT( KSqlFromTranslations, "SELECT * FROM %S");
    _LIT( KSqlWhereFKeys, " WHERE %S = %d AND %S = %d" );
    query.Format( KSqlFromTranslations(), iTableNameTranslations );
    if ( !iReversed )
        query.AppendFormat( KSqlWhereFKeys(), &KColSourceFk, aEntryId, &KColTargetFk, aTargetId );
    else
        query.AppendFormat( KSqlWhereFKeys(), &KColTargetFk, aTargetId, &KColSourceFk, aEntryId );

    EvaluateViewL( aView, query );

    return aView.FirstL();
}

// ----------------------------------------------------------

//TInt CFiruData::AddExampleL(
//    TInt aEntryId,
//    TInt aTranslationId,
//    const TDesC& aExample )
//{
//    return FiruDbSchema::AddExampleL( iTableExamples, aEntryId, aTranslationId, aExample );
//}

// ----------------------------------------------------------

TInt CFiruData::NumEntriesL() const
{
    if ( !iReversed )
        return iTableSources.CountL();
    else
        return iTableTargets.CountL();
}

// ----------------------------------------------------------

void CFiruData::GetEntriesL(
    RPointerArray<CFiruEntry>& aEntries,
    const TDesC& aPattern,
    TInt aMaximum )
{
    aEntries.ResetAndDestroy();

    TInt count = 0;
    TDbSeekKey key( aPattern );
    RDbTable* table = iReversed ? &iTableTargets : &iTableSources;

    table->SeekL( key, RDbTable::EGreaterEqual );
    while ( table->AtRow() && count < aMaximum )
    {
        table->GetL();
        CFiruEntry* entry = CreateEntryLC( *table );
        aEntries.AppendL( entry );
        CleanupStack::Pop( entry );
        count++;
        table->NextL();
    }
}

// ----------------------------------------------------------

void CFiruData::GetMoreEntriesL( RPointerArray<CFiruEntry>& aEntries, TInt aMaximum )
{
    TInt count = 0;
    RDbTable* table = iReversed ? &iTableTargets : &iTableSources;
    while ( table->AtRow() && count < aMaximum )
    {
        table->GetL();
        CFiruEntry* entry = CreateEntryLC( *table );
        aEntries.AppendL( entry );
        CleanupStack::Pop( entry );
        count++;
        table->NextL();
    }
}

// ----------------------------------------------------------

CFiruEntry* CFiruData::CreateEntryLC( RDbRowSet& aView )
{
    TInt id = aView.ColUint32( KColumnId );
    TPtrC text( aView.ColDes16( KColumnText ) );

    return CFiruEntry::NewLC( id, text );
}

// ----------------------------------------------------------

void CFiruData::GetTranslationsL( CFiruEntry& aEntry )
{
    RDbTable* targets = iReversed ? &iTableSourcesById : &iTableTargetsById;

    RDbView view;
    CleanupClosePushL( view );

    TBuf<128> query;
    query.Format( KSqlViewWhere(), iTableNameTranslations );
    if ( !iReversed )
        query.AppendFormat( KSqlEqual(), &KColSourceFk, aEntry.Id() );
    else
        query.AppendFormat( KSqlEqual(), &KColTargetFk, aEntry.Id() );

    EvaluateViewL( view, query );

    view.FirstL();
    while ( !view.AtEnd() )
    {
        view.GetL();

        TInt counter = view.ColInt( iReversed ? KColumnReverseCount : KColumnForwardCount );
        TInt rate = view.ColInt( iReversed ? KColumnReverseMark : KColumnForwardMark );

        TDbSeekKey key( view.ColUint( iReversed ? KColumnSourceFk : KColumnTargetFk ) );
        if ( targets->SeekL( key ) )
        {
            targets->GetL();
            CFiruEntry* entry = CreateEntryLC( *targets );
            CFiruTranslation* trans = new ( ELeave ) CFiruTranslation( entry, counter, rate );
            CleanupStack::Pop( entry );
            CleanupStack::PushL( trans );
            aEntry.AddTranslationL( trans );
            CleanupStack::Pop( trans );
        }
        view.NextL();
    }

    CleanupStack::PopAndDestroy( &view );
}

// ----------------------------------------------------------

CFiruExercise* CFiruData::CreateExerciseL( TInt numTotalTests, TInt numNewTests, TInt numVariants )
{
    CFiruExercise* excercise = new ( ELeave ) CFiruExercise;
    CleanupStack::PushL( excercise );

    // T questions with T answers
    // (T-1)*W wrong translations
    // Add all source->target tests and then target->source tests

    RDbView view;
    TBuf<128> query;

    RPointerArray<CFiruEntry> questions, answers;
    questions.Reserve( numTotalTests );
    answers.Reserve( numTotalTests );
    CleanupStack::PushL( TCleanupItem( ResetAndDestroyDataEntryArray, &questions ) );
    CleanupStack::PushL( TCleanupItem( ResetAndDestroyDataEntryArray, &answers ) );

    // 1. Get worst old entries

    _LIT( KSqlWorstOldEntries,
        "SELECT * FROM %S WHERE (%S < %d) AND (%S > 0) ORDER BY %S");
    if ( !iReversed )
    {
        query.Format( KSqlWorstOldEntries(), iTableNameTranslations, &KColForward, KMinGoodRate,
            &KColFwdCounter, &KColForward );
    }
    else
    {
        query.Format( KSqlWorstOldEntries(), iTableNameTranslations, &KColReverse, KMinGoodRate,
            &KColRevCounter, &KColReverse );
    }

    CleanupClosePushL( view );
    EvaluateViewL( view, query );
    ReadTopEntriesL( view, questions, numTotalTests - numNewTests, iReversed );
    CleanupStack::PopAndDestroy( &view );

    // 2. Get most rare entries ( the rest )

    query.Format( KSqlViewAll(), iTableNameTranslations );
    if ( !iReversed )
        query.AppendFormat( KSqlOrder(), &KColFwdCounter );
    else
        query.AppendFormat( KSqlOrder(), &KColRevCounter );

    CleanupClosePushL( view );
    EvaluateViewL( view, query );
    ReadRandomEntriesL( view, questions, numTotalTests - questions.Count(), iReversed );
    CleanupStack::PopAndDestroy( &view );

    // 3. Get wrong results

    TInt numWrongVariants = numTotalTests * ( numVariants - 1 );

    RPointerArray<CFiruEntry> wrongTranslations;
    wrongTranslations.Reserve( numWrongVariants );
    CleanupStack::PushL( TCleanupItem( ResetAndDestroyDataEntryArray, &wrongTranslations ) );

    query.Format( KSqlViewAll, iTableNameTranslations );

    CleanupClosePushL( view );
    EvaluateViewL( view, query );
    ReadRandomEntriesL( view, wrongTranslations, numWrongVariants, !iReversed ); //answers
    CleanupStack::PopAndDestroy( &view );

    // 4. Collect all together

    while ( questions.Count() )
    {
        CFiruTest* test = CFiruTest::NewLC( questions[0], answers[0], iReversed );
        questions.Remove( 0 );
        answers.Remove( 0 );

        for ( int i = 0; ( i < numVariants - 1 ) && wrongTranslations.Count(); i++ )
        {
            CFiruEntry* entry = wrongTranslations[0];
            test->AddWrongVariantL( entry->Text() );
            wrongTranslations.Remove( 0 );
            delete entry;
        }

        excercise->AddTestL( test );
        CleanupStack::Pop( test );
    }

    CleanupStack::PopAndDestroy( &wrongTranslations );
    CleanupStack::PopAndDestroy(); // answers
    CleanupStack::PopAndDestroy(); // questions

    CleanupStack::Pop( excercise );
    return excercise;
}

// ----------------------------------------------------------

void CFiruData::ReadRandomEntriesL(
    RDbRowSet& aView,
    RPointerArray<CFiruEntry>& entries,
    TInt aCount,
    TBool aReversed )
{
    TInt totalCount = aView.CountL();
    aCount = Min( aCount, totalCount );

    // get indexes of random records
    RArray<TInt> rnds;
    CleanupClosePushL( rnds );

    while ( rnds.Count() < aCount )
    {
        TInt rnd = totalCount * Math::FRand( gRandSeed );
        TInt err = rnds.InsertInOrder( rnd ); // no duplicates
        if ( err && err != KErrAlreadyExists )
            User::Leave( err );
    }

    aView.FirstL();
    TInt count = 0;
    TInt index = 0;
    while ( !aView.AtEnd() && count < aCount )
    {
        if ( rnds.FindInOrder( index++ ) >= 0 )
        {
            aView.GetL();
            CFiruEntry* entry = CreateEntryLC( aView );

            TInt pos = ( entries.Count() + 1 ) * Math::FRand( gRandSeed );
            entries.InsertL( entry, pos );
            CleanupStack::Pop( entry );

            count++;
        }
        aView.NextL();
    }

    CleanupStack::PopAndDestroy( &rnds );
}

// ----------------------------------------------------------

void CFiruData::ReadTopEntriesL(
    RDbRowSet& aView,
    RPointerArray<CFiruEntry>& entries,
    TInt aCount,
    TBool aReversed )
{
    aView.FirstL();
    TInt index = 0;
    while ( !aView.AtEnd() && aCount > 0 )
    {
        aView.GetL();

        CFiruEntry* q = CreateEntryLC( aView );
        entries.AppendL( q );
        CleanupStack::Pop( q );

        aCount--;
        aView.NextL();
    }
}

// ----------------------------------------------------------

void CFiruData::SaveTestResultL( const CFiruTest& aTest )
{
    if ( aTest.IsPassed() && aTest.NumMistakes() == 0 )
    {
        AdjustMarkL( aTest.Question().Id(), aTest.Answer().Id(), +1, ETrue, ETrue );
    }
    else
    {
        AdjustMarkL( aTest.Question().Id(), aTest.Answer().Id(), -1, EFalse, ETrue );
    }
}

// ----------------------------------------------------------

void CFiruData::AddEntryToLearningSetL( TInt aEntryId )
{
    CFiruEntry* entry = CFiruEntry::NewLC( aEntryId, _L("") );
    GetTranslationsL( *entry );
    for ( int i = 0; i < entry->NumTranslations(); ++i )
    {
        CFiruTranslation* trans = entry->Translation( i );
        AdjustMarkL( aEntryId, trans->Entry().Id(), -3, EFalse, EFalse );
    }
    CleanupStack::PopAndDestroy( entry );
}

// ----------------------------------------------------------

void CFiruData::AdjustMarkL(
    TInt aEntryID,
    TInt aTargetID,
    TInt aMark,
    TBool aShiftMark,
    TBool aUpdateCounter )
{
    RDbView view;
    CleanupClosePushL( view );

    if ( FindTranslationRowL( view, aEntryID, aTargetID ) )
    {
        view.GetL();
        TInt mark = view.ColInt( iReversed ? KColumnReverseMark : KColumnForwardMark );
        TInt counter = view.ColInt( iReversed ? KColumnReverseCount : KColumnForwardCount );

        view.UpdateL();
        if ( aShiftMark )
            mark = aMark + mark;
        else
            mark = aMark;

        view.SetColL( iReversed ? KColumnReverseMark : KColumnForwardMark, mark );
        if ( aUpdateCounter )
        {
            counter++;
            view.SetColL( iReversed ? KColumnReverseCount : KColumnForwardCount, counter );
        }
        view.PutL();
    }
    CleanupStack::PopAndDestroy( &view );
}

// ----------------------------------------------------------

void CFiruData::ResetStatsL()
{

}

// ----------------------------------------------------------

void CFiruData::ClearDictionaryL()
{
    //    HBufC* query = HBufC::NewLC( KSqlDeleteAll().Length() + iTableName->Length() );
    //    query->Des().Format( KSqlDeleteAll, &iTableName );
    //    iDb.Execute( *query );
    //    CleanupStack::PopAndDestroy( query );
}

// ----------------------------------------------------------

void CFiruData::CompactL()
{
    User::LeaveIfError( iDb.Compact() );
}

// ----------------------------------------------------------

CFiruData::Stats CFiruData::GetStats()
{
    Stats stats;

    // total
    RDbTable* entries = iReversed ? &iTableTargets : &iTableSources;
    stats.iTotalEntries = entries->CountL();

    TBuf<128> query;
    // not asked
    _LIT( KSqlNotAskedEntries, "SELECT * FROM %S WHERE %S = 0");
    if ( !iReversed )
    {
        query.Format( KSqlNotAskedEntries(), iTableNameTranslations, &KColFwdCounter );
    }
    else
    {
        query.Format( KSqlNotAskedEntries(), iTableNameTranslations, &KColRevCounter );
    }

    RDbView view;
    CleanupClosePushL( view );
    EvaluateViewL( view, query );
    stats.iNotAsked = view.CountL();
    CleanupStack::PopAndDestroy( &view );

    // positives

    // negatives

    return stats;
}

// ----------------------------------------------------------
// ----------------------------------------------------------

CFiruTest::CFiruTest() :
    iCorrectTranslationIndex( -1 )
{
}

// ----------------------------------------------------------

CFiruTest::~CFiruTest()
{
    delete iQuestion;
    delete iAnswer;
    iVariants.ResetAndDestroy();
}

// ----------------------------------------------------------

CFiruTest* CFiruTest::NewLC( CFiruEntry* aQuestion, CFiruEntry* aAnswer, TBool aReversed )
{
    ASSERT( aQuestion );
    ASSERT( aAnswer );

    CFiruTest* self = new ( ELeave ) CFiruTest;
    CleanupStack::PushL( self );

    self->iQuestion = aQuestion;
    self->iAnswer = aAnswer;
    self->iReversed = aReversed;

    HBufC* text = aAnswer->Text().AllocLC();
    self->iVariants.AppendL( text );
    CleanupStack::Pop( text );
    self->iCorrectTranslationIndex = 0;

    return self;
}

// ----------------------------------------------------------

void CFiruTest::AddWrongVariantL( const TDesC& aText )
{
    // replace correct answer with wrong variant
    // and move correct one to new random place

    HBufC* correct = iVariants[iCorrectTranslationIndex];

    HBufC* text = aText.AllocLC();
    iVariants[iCorrectTranslationIndex] = text;
    CleanupStack::Pop( text );

    TInt newIndex = ( iVariants.Count() + 1 ) * Math::FRand( gRandSeed );
    iVariants.InsertL( correct, newIndex );
    iCorrectTranslationIndex = newIndex;
}

// ----------------------------------------------------------

TPtrC CFiruTest::QuestionText() const
{
    return iQuestion->Text();
}

// ----------------------------------------------------------

TPtrC CFiruTest::AnswerText() const
{
    return iAnswer->Text();
}

// ----------------------------------------------------------

TBool CFiruTest::TryVariant( TUint aIndex )
{
    // assuming there are no correct answers in wrong variants
    if ( aIndex == iCorrectTranslationIndex )
    {
        iPassed = ETrue;
        return ETrue;
    }
    else
    {
        iNumMistakes++;
        return EFalse;
    }
}

// ----------------------------------------------------------

TBool CFiruTest::TryAnswer( const TDesC& aText )
{
    if ( aText.MatchC( AnswerText() ) == 0 )
    {
        iPassed = ETrue;
        return ETrue;
    }
    else
    {
        iNumMistakes++;
        return EFalse;
    }
}

// ----------------------------------------------------------

const RPointerArray<HBufC>& CFiruTest::AllVariants() const
{
    return iVariants;
}

// ----------------------------------------------------------
// ----------------------------------------------------------

CFiruExercise::CFiruExercise()
{
}

// ----------------------------------------------------------

CFiruExercise::~CFiruExercise()
{
    iTests.ResetAndDestroy();
}

// ----------------------------------------------------------

void CFiruExercise::AddTestL( CFiruTest* aTest )
{
    iTests.AppendL( aTest );
}

// ----------------------------------------------------------

CFiruTest* CFiruExercise::NextTest()
{
    if ( iCounter < iTests.Count() )
        return iTests[iCounter++];
    else
        return NULL;
}

// ----------------------------------------------------------

CFiruExercise::Stats CFiruExercise::GetStats() const
{
    TInt passed = 0, asked = 0;
    for ( int i = 0; i < iTests.Count(); ++i )
    {
        CFiruTest* test = iTests[i];
        if ( test->IsPassed() && test->NumMistakes() == 0 )
        {
            asked++;
            passed++;
        }
        else if ( test->NumMistakes() > 0 )
        {
            asked++;
        }
    }
    return Stats( passed, asked );
}

// ----------------------------------------------------------

//void CFiruData::DebugFindTargetDuplicatesL()
//{
//    TInt dups = 0;
//    HBufC* previous = NULL;
//    iTableTranslations.FirstL();
//    while ( !iTableTranslations.AtEnd() )
//    {
//        iTableTranslations.GetL();
//        TPtrC text = iTableTranslations.ColDes( KColumnTarget );
//        if ( previous )
//        {
//            if ( text.Compare( *previous ) == 0 )
//            {
//                RDebug::Print( _L("Dup found: %S"), &text );
//                dups++;
//            }
//            CleanupStack::PopAndDestroy( previous );
//        }
//        previous = text.AllocLC();
//        iTableTranslations.NextL();
//    }
//    iTableTranslations.FirstL();
//    CleanupStack::PopAndDestroy( previous );
//    RDebug::Print( _L("Found %d dups in %d records"), dups, iTableTranslations.CountL() );
//}

void CFiruData::DumpL()
{
    const TInt KMaxRecords = 10;
    TInt i = KMaxRecords;
    RDebug::Print(_L("-= Sources =-"));
    iTableSources.FirstL();
    while ( iTableSources.AtRow() && i-- > 0 )
    {
        iTableSources.GetL();
        CFiruEntry* entry = CreateEntryLC( iTableSources );
        RDebug::Print(_L("%2d %S"), entry->Id(), &(entry->Text()));
        CleanupStack::PopAndDestroy( entry );
        iTableSources.NextL();
    }

    i = KMaxRecords;
    RDebug::Print(_L("-= Targets =-"));
    iTableTargets.FirstL();
    while ( iTableTargets.AtRow() && i-- > 0 )
    {
        iTableTargets.GetL();
        CFiruEntry* entry = CreateEntryLC( iTableTargets );
        RDebug::Print(_L("%2d %S"), entry->Id(), &(entry->Text()));
        CleanupStack::PopAndDestroy( entry );
        iTableTargets.NextL();
    }

    i = KMaxRecords;
    RDebug::Print(_L("-= Traslations =-"));
    iTableTranslations.FirstL();
    while ( iTableSources.AtRow() && i-- > 0 )
    {
        iTableTranslations.GetL();
        TInt sid = iTableTranslations.ColUint32( KColumnSourceFk );
        TInt tid = iTableTranslations.ColUint32( KColumnTargetFk );
        RDebug::Print(_L("%2d %2d"), sid, tid );
        iTableTranslations.NextL();
    }
}
