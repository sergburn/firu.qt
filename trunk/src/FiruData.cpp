#include <e32math.h>
#include <d32dbms.h>

#include "firudata.h"

_LIT( KDatabaseFile, "c:\\private\\E0EDB1C1\\dicts.db" );
_LIT( KDirTableNameFmt, "from_%02d_to_%02d" );
_LIT( KDirTableIdIndexNameFmt, "ind_%S_id" );
_LIT( KDirTableSrcIndexNameFmt, "ind_%S_src" );
_LIT( KDirTableTrgIndexNameFmt, "ind_%S_trg" );

_LIT( KSqlViewAll, "SELECT * FROM %S");
_LIT( KSqlViewWhere, "SELECT * FROM %S WHERE ");
_LIT( KSqlDeleteAll, "DELETE FROM %S");
_LIT( KSqlLike, " %S LIKE '%S' ");
_LIT( KSqlLessEqual, " %S <= %d ");
_LIT( KSqlEqual, " %S = %d ");
_LIT( KSqlOrder, " ORDER BY %S");
_LIT( KWildCardStar, "*" );

// first version employs simplest data structure
// single table of following format
// id - record id, int32, autoinc
// source - source text, unicode
// target - target text, unicode
// forward - from source to target association mark, int
// reverse - from target to source association mark, int

_LIT( KColId, "id" );
_LIT( KColSource, "source" );
_LIT( KColTarget, "target" );
_LIT( KColForward, "forward" );
_LIT( KColReverse, "reverse" );
_LIT( KColFwdCounter, "fwd_count" );
_LIT( KColRevCounter, "rev_count" );

const TInt KColumnId = 1;
const TInt KColumnSource = 2;
const TInt KColumnTarget = 3;
const TInt KColumnForward = 4;
const TInt KColumnReverse = 5;
const TInt KColumnFwdCounter = 6;
const TInt KColumnRevCounter = 7;

const TInt KMaxTextColumnLength = 64;
const TInt KMaxTextColIndexLength = 16;

const TInt KIntStringMaxLength = 11;

const TInt KMinGoodRate = 3;

TInt64 gRandSeed;

// ----------------------------------------------------------

void ResetAndDestroyDataEntryArray( TAny* aPtr )
{
    RPointerArray<CFiruDataEntry>* array =
        reinterpret_cast<RPointerArray<CFiruDataEntry>*>(aPtr);
    array->ResetAndDestroy();
}

// ----------------------------------------------------------
// ----------------------------------------------------------

CFiruDataEntry* CFiruDataEntry::NewLC(
    TInt aId, const TDesC& aText,
    TInt aCounter, TInt aRate )
{
    CFiruDataEntry* self = new (ELeave) CFiruDataEntry;
    CleanupStack::PushL( self );
    self->iId = aId;
    self->iText = aText.AllocL();
    self->iCounter = aCounter;
    self->iRate = aRate;
    self->iTranslations = new ( ELeave ) CDesC16ArrayFlat( 1 );
    return self;
}

// ----------------------------------------------------------

CFiruDataEntry::CFiruDataEntry()
{
}

// ----------------------------------------------------------

CFiruDataEntry::~CFiruDataEntry()
{
    delete iText;
    delete iTranslations;
}

// ----------------------------------------------------------

TInt CFiruDataEntry::Id() const
{
    return iId;
}

// ----------------------------------------------------------

TPtrC CFiruDataEntry::Text() const
{
    return *iText;
}

// ----------------------------------------------------------

TUint CFiruDataEntry::NumTranslations() const
{
    return iTranslations->Count();
}

// ----------------------------------------------------------

TPtrC CFiruDataEntry::Translation( TUint aIndex ) const
{
    if ( aIndex < iTranslations->Count() )
        return iTranslations->MdcaPoint( aIndex );
    else
        return TPtrC();
}

// ----------------------------------------------------------

const MDesC16Array& CFiruDataEntry::Translations() const
{
    return *iTranslations;
}

// ----------------------------------------------------------

void CFiruDataEntry::AddTranslationL( const TDesC& aText )
{
    iTranslations->AppendL( aText );
}

// ----------------------------------------------------------
// ----------------------------------------------------------

CFiruData* CFiruData::NewL( RFs& aFs )
{
    CFiruData* self = new (ELeave) CFiruData( aFs );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
}

// ----------------------------------------------------------

CFiruData::CFiruData( RFs& aFs ) :
    iFs(aFs)
{
    iSourceLang = ELangFinnish;
    iTargetLang = ELangRussian;
}

// ----------------------------------------------------------

CFiruData::~CFiruData()
{
    delete iTableName;
    iTable.Close();
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
    iTable.Close();

    iTableName = DictionaryTableNameLC( aInputLanguage, aOutputLanguage );
    CleanupStack::Pop( iTableName );

    CreateDictionaryL( *iTableName );

    iSourceLang = aInputLanguage;
    iTargetLang = aOutputLanguage;
    iReversed = EFalse;

    iTable.Open( iDb, *iTableName, RDbTable::EUpdatable );
    SetTableIndexL();
}

// ----------------------------------------------------------

void CFiruData::EvaluateViewL( RDbView& aView, const TDesC& aQuery )
{
    TInt err = aView.Prepare( iDb, aQuery );
    RDebug::Print( _L("Prepare query '%S', error %d"), &aQuery, err );
    User::LeaveIfError( err );
    err = aView.EvaluateAll();
    RDebug::Print( _L("Evaluate query '%S', error %d, count %d"), &aQuery, err, aView.CountL() );
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
    HBufC* indexName = NULL;
    if ( iReversed )
        indexName = ReverseIndexNameLC( *iTableName );
    else
        indexName = ForwardIndexNameLC( *iTableName );

    User::LeaveIfError( iTable.SetIndex( *indexName ) );
    CleanupStack::PopAndDestroy( indexName );
}

// ----------------------------------------------------------

void CFiruData::GetLanguagesL( TLanguage& aInputLanguage, TLanguage& aOutputLanguage ) const
{
    aInputLanguage = iSourceLang;
    aOutputLanguage = iTargetLang;
}

// ----------------------------------------------------------

void CFiruData::CreateDictionaryL( const TDesC& aDictName )
{
    CDbTableNames* tables = iDb.TableNamesL();
    CleanupStack::PushL( tables );

    TBool tableExists = EFalse;
    for ( int i = 0; i < tables->Count(); i++ )
    {
        if ( (*tables)[i].Compare( aDictName ) == 0 )
        {
            tableExists = ETrue;
            break;
        }
    }

    if ( !tableExists )
    {
        CDbColSet* cols = CDbColSet::NewLC();
        TDbCol id( KColId, EDbColUint32);
        id.iAttributes = TDbCol::EAutoIncrement | TDbCol::ENotNull;
        cols->AddL( id );
        cols->AddL( TDbCol( KColSource, EDbColText16, KMaxTextColumnLength ) );
        cols->AddL( TDbCol( KColTarget, EDbColText16, KMaxTextColumnLength ) );
        cols->AddL( TDbCol( KColForward, EDbColInt16 ) );
        cols->AddL( TDbCol( KColReverse, EDbColInt16 ) );
        cols->AddL( TDbCol( KColFwdCounter, EDbColInt16 ) );
        cols->AddL( TDbCol( KColRevCounter, EDbColInt16 ) );
        User::LeaveIfError( iDb.CreateTable( aDictName, *cols ) );
        CleanupStack::PopAndDestroy( cols );

        TBuf<64> indexName;

        // ID index
        CDbKey* key = CDbKey::NewLC();
        key->AddL( TDbKeyCol( KColId ) );
        indexName.Format( KDirTableIdIndexNameFmt, &aDictName );
        User::LeaveIfError( iDb.CreateIndex( indexName, aDictName, *key ) );

        // Source text index
        key->Clear();
        key->AddL( TDbKeyCol( KColSource, KMaxTextColIndexLength ) );
        HBufC* srcIndex = ForwardIndexNameLC( aDictName );
        User::LeaveIfError( iDb.CreateIndex( *srcIndex, aDictName, *key ) );
        CleanupStack::PopAndDestroy( srcIndex );

        // Target text index
        key->Clear();
        key->AddL( TDbKeyCol( KColTarget, KMaxTextColIndexLength ) );
        HBufC* trgIndex = ReverseIndexNameLC( aDictName );
        User::LeaveIfError( iDb.CreateIndex( *trgIndex, aDictName, *key ) );
        CleanupStack::PopAndDestroy( trgIndex );

        CleanupStack::PopAndDestroy( key );
    }

    CleanupStack::PopAndDestroy( tables );
}

// ----------------------------------------------------------

void CFiruData::AddPair( const TDesC& aEntry, const TDesC& aTranslation )
{
    TRAPD( err, AddPairL( aEntry, aTranslation ) );
    if ( err )
    {
//        iView.Cancel();
        RDebug::Print( _L("Couldn't (%d) add pair: %S - %S"), err, &aEntry, &aTranslation );
    }
}

// ----------------------------------------------------------

void CFiruData::AddPairL( const TDesC& aEntry, const TDesC& aTranslation )
{
    iTable.InsertL();
    iTable.SetColL( KColumnSource, aEntry.Left( KMaxTextColumnLength ) );
    iTable.SetColL( KColumnTarget, aTranslation.Left( KMaxTextColumnLength ) );
    iTable.SetColL( KColumnForward, 0 );
    iTable.SetColL( KColumnReverse, 0 );
    iTable.SetColL( KColumnFwdCounter, 0 );
    iTable.SetColL( KColumnRevCounter, 0 );
    iTable.PutL();

//    iDb.UpdateStats();
//    RDbNamedDatabase::TSize size = iDb.Size();
//    RDebug::Print(_L("Firu: added pair; db size %d, usage %d"), size.iSize, size.iUsage );
}

// ----------------------------------------------------------

TInt CFiruData::NumEntriesL() const
{
    return iTable.CountL();
}

// ----------------------------------------------------------

//void CFiruData::SetFilterL( const TDesC& aFilter )
//{
//    HBufC* filter = HBufC::NewLC( aFilter.Length() + 1 );
//    filter->Des().Copy( aFilter );
//    filter->Des().Append( KWildCardStar );
//
//    HBufC* query = HBufC::NewLC( filter->Length() + KSqlViewWhere().Length() + KSqlLike().Length() );
//    query->Des().Format( KSqlViewWhere, &iTableName );
//    if ( !iReversed )
//    {
//        query->Des().AppendFormat( KSqlLike, &KColSource, &aFilter );
//        query->Des().AppendFormat( KSqlOrder, &KColSource );
//    }
//    else
//    {
//        query->Des().AppendFormat( KSqlLike, &KColTarget, &aFilter );
//        query->Des().AppendFormat( KSqlOrder, &KColTarget );
//    }
//    EvaluateViewL( iView, *query );
//    CleanupStack::PopAndDestroy( filter );
//}
//
//// ----------------------------------------------------------
//// ----------------------------------------------------------
////
//void CFiruData::ResetFilterL()
//{
//    TBuf<128> query;
//    query.Format( KSqlViewAll, &iTableName );
//    if ( !iReversed )
//    {
//        query.AppendFormat( KSqlOrder, &KColSource );
//    }
//    else
//    {
//        query.AppendFormat( KSqlOrder, &KColTarget );
//    }
//    EvaluateViewL( iView, query );
//}

// ----------------------------------------------------------

void CFiruData::GetEntriesL(
    RPointerArray<CFiruDataEntry>& aEntries,
    const TDesC& aPattern,
    TInt aMaximum )
{
    aEntries.ResetAndDestroy();

    TInt count = 0;
    TDbSeekKey key( aPattern );
    iTable.SeekL( key, RDbTable::EGreaterEqual );
    while ( iTable.AtRow() && count < aMaximum )
    {
        iTable.GetL();
        CFiruDataEntry* entry = CreateEntryLC( iTable, iReversed );
        aEntries.AppendL( entry );
        CleanupStack::Pop( entry );
        count++;
        iTable.NextL();
    }
}

// ----------------------------------------------------------

void CFiruData::GetMoreEntriesL(
    RPointerArray<CFiruDataEntry>& aEntries,
    TInt aMaximum )
{
    TInt count = 0;
    while ( iTable.AtRow() && count < aMaximum )
    {
        iTable.GetL();
        CFiruDataEntry* entry = CreateEntryLC( iTable, iReversed );
        aEntries.AppendL( entry );
        CleanupStack::Pop( entry );
        count++;
        iTable.NextL();
    }
}

// ----------------------------------------------------------

CFiruDataEntry* CFiruData::CreateEntryLC(
    RDbRowSet& aView,
    TBool aReverse )
{
    TInt id = aView.ColUint32( KColumnId );

    TPtrC text;
    TPtrC trans;
    TInt counter;
    TInt rate;

    if ( !aReverse )
    {
        text.Set( aView.ColDes16( KColumnSource ) );
        trans.Set( aView.ColDes16( KColumnTarget ) );
        counter = aView.ColInt( KColumnFwdCounter );
        rate = aView.ColInt( KColumnForward );
    }
    else
    {
        text.Set( aView.ColDes16( KColumnTarget ) );
        trans.Set( aView.ColDes16( KColumnSource ) );
        counter = aView.ColInt( KColumnRevCounter );
        rate = aView.ColInt( KColumnReverse );
    }

    CFiruDataEntry* entry = CFiruDataEntry::NewLC( id, text, counter, rate );
    entry->AddTranslationL( trans );
    return entry;
}

// ----------------------------------------------------------

CFiruDataEntry* CFiruData::TranslationLC( TInt aEntryID )
{
    RDbView view;
    CleanupClosePushL( view );
    FindEntryRowL( view, aEntryID );

    view.GetL();
    CFiruDataEntry* entry = CreateEntryLC( view, !iReversed );
    CleanupStack::Pop( entry );
    CleanupStack::PopAndDestroy( &view );
    CleanupStack::PushL( entry );
    return entry;
}

// ----------------------------------------------------------

void CFiruData::FindEntryRowL( RDbView& aView, TInt aEntryID )
{
    TBuf<128> query;
    query.Format( KSqlViewWhere, iTableName );
    query.AppendFormat( KSqlEqual, &KColId, aEntryID );

    EvaluateViewL( aView, query );
    aView.FirstL();
    if ( aView.AtEnd() )
    {
        User::Leave( KErrNotFound );
    }
}

// ----------------------------------------------------------

CFiruExercise* CFiruData::CreateExerciseL(
    TInt numTotalTests,
    TInt numNewTests,
    TInt numVariants )
{
    CFiruExercise* excercise = new (ELeave) CFiruExercise;
    CleanupStack::PushL( excercise );

    RDbView view;
    TBuf<128> query;

    RPointerArray<CFiruDataEntry> questions;
    questions.Reserve( numTotalTests );
    CleanupStack::PushL(
        TCleanupItem( ResetAndDestroyDataEntryArray, &questions ) );

    // 1. Get worst old entries

    _LIT( KSqlWorstOldEntries,
        "SELECT * FROM %S WHERE (%S < %d) AND (%S > 0) ORDER BY %S");
    if ( !iReversed )
    {
        query.Format( KSqlWorstOldEntries, iTableName,
            &KColForward, KMinGoodRate,
            &KColFwdCounter,
            &KColForward );
    }
    else
    {
        query.Format( KSqlWorstOldEntries, iTableName,
            &KColReverse, KMinGoodRate,
            &KColRevCounter,
            &KColReverse );
    }

    CleanupClosePushL( view );
    EvaluateViewL( view, query );
    ReadTopEntriesL( view, questions, numTotalTests - numNewTests, iReversed );
    CleanupStack::PopAndDestroy( &view );

    // 2. Get most rare entries ( the rest )

    query.Format( KSqlViewAll, iTableName );
    if ( !iReversed )
        query.AppendFormat( KSqlOrder, &KColFwdCounter );
    else
        query.AppendFormat( KSqlOrder, &KColRevCounter );

    CleanupClosePushL( view );
    EvaluateViewL( view, query );
    ReadRandomEntriesL( view, questions, numTotalTests - questions.Count(), iReversed );
    CleanupStack::PopAndDestroy( &view );

    // 3. Get wrong results

    TInt numWrongVariants = numTotalTests * ( numVariants - 1 );

    RPointerArray<CFiruDataEntry> wrongTranslations;
    wrongTranslations.Reserve( numWrongVariants );
    CleanupStack::PushL(
        TCleanupItem( ResetAndDestroyDataEntryArray, &wrongTranslations ) );

    query.Format( KSqlViewAll, iTableName );

    CleanupClosePushL( view );
    EvaluateViewL( view, query );
    ReadRandomEntriesL( view, wrongTranslations, numWrongVariants, !iReversed ); //answers
    CleanupStack::PopAndDestroy( &view );

    // 4. Collect all together

    while ( questions.Count() )
    {
        CFiruTest* test = CFiruTest::NewLC( questions[0], iReversed );
        questions.Remove( 0 );

        for ( int i = 0;
            ( i < numVariants - 1 ) && wrongTranslations.Count();
            i++ )
        {
            CFiruDataEntry* entry = wrongTranslations[0];
            test->AddWrongVariantL( entry->Text() );
            wrongTranslations.Remove( 0 );
            delete entry;
        }

        excercise->AddTestL( test );
        CleanupStack::Pop( test );
    }

    CleanupStack::PopAndDestroy( &wrongTranslations );
    CleanupStack::PopAndDestroy( &questions );

    CleanupStack::Pop( excercise );
    return excercise;
}

// ----------------------------------------------------------

void CFiruData::ReadRandomEntriesL(
    RDbRowSet& aView,
    RPointerArray<CFiruDataEntry>& entries,
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
        TInt rnd = totalCount * Math::FRand( iRandSeed );
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
            CFiruDataEntry* entry = CreateEntryLC( aView, aReversed );

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
    RPointerArray<CFiruDataEntry>& entries,
    TInt aCount,
    TBool aReversed )
{
    aView.FirstL();
    TInt index = 0;
    while ( !aView.AtEnd() && aCount > 0 )
    {
        aView.GetL();

        CFiruDataEntry* q = CreateEntryLC( aView, aReversed );
        entries.AppendL( q );
        CleanupStack::Pop( q );

        aCount--;
        aView.NextL();
    }
}

// ----------------------------------------------------------
//
void CFiruData::SaveTestResultL( const CFiruTest& aTest )
{
    if ( aTest.IsPassed() && aTest.NumMistakes() == 0 )
    {
        AdjustMarkL( aTest.Entry().Id(), +1, aTest.IsReversed(), ETrue );
    }
    else
    {
        AdjustMarkL( aTest.Entry().Id(), -1, aTest.IsReversed(), ETrue );
    }
}

// ----------------------------------------------------------

void CFiruData::AdjustMarkL( TInt aEntryID, TInt aCorrection, TBool aReversed, TBool aUpdateCounter )
{
    RDbView view;
    CleanupClosePushL( view );

    FindEntryRowL( view, aEntryID );

    view.GetL();
    TInt16 mark = aCorrection + view.ColInt16( aReversed ? KColumnReverse : KColumnForward );
    TInt16 counter = 1 + view.ColInt16( aReversed ? KColumnRevCounter : KColumnFwdCounter );

    view.UpdateL();
    view.SetColL( aReversed ? KColumnReverse : KColumnForward, mark );
    if ( aUpdateCounter )
    {
        view.SetColL( aReversed ? KColumnRevCounter : KColumnFwdCounter, counter );
    }
    view.PutL();

    CleanupStack::PopAndDestroy( &view );
}

// ----------------------------------------------------------

void CFiruData::ResetStatsL()
{

}

// ----------------------------------------------------------

void CFiruData::ClearDictionaryL()
{
    HBufC* query = HBufC::NewLC( KSqlDeleteAll().Length() + iTableName->Length() );
    query->Des().Format( KSqlDeleteAll, &iTableName );
    iDb.Execute( *query );
    CleanupStack::PopAndDestroy( query );
}

// ----------------------------------------------------------

HBufC* CFiruData::DictionaryTableNameLC( TLanguage aInputLanguage, TLanguage aOutputLanguage )
{
    HBufC* name = HBufC::NewLC( KDirTableNameFmt().Length() + 2 * KIntStringMaxLength );
    TPtr buf = name->Des();
    buf.Format( KDirTableNameFmt, aInputLanguage, aOutputLanguage );
    return name;
}

// ----------------------------------------------------------

HBufC* CFiruData::ForwardIndexNameLC( const TDesC& aTableName )
{
    HBufC* name = HBufC::NewLC( KDirTableSrcIndexNameFmt().Length() + 2 * KIntStringMaxLength );
    TPtr buf = name->Des();
    buf.Format( KDirTableSrcIndexNameFmt, &aTableName );
    return name;
}

// ----------------------------------------------------------

HBufC* CFiruData::ReverseIndexNameLC( const TDesC& aTableName )
{
    HBufC* name = HBufC::NewLC( KDirTableTrgIndexNameFmt().Length() + 2 * KIntStringMaxLength );
    TPtr buf = name->Des();
    buf.Format( KDirTableTrgIndexNameFmt, &aTableName );
    return name;
}

// ----------------------------------------------------------

void CFiruData::CompactL()
{
    User::LeaveIfError( iDb.Compact() );
}

// ----------------------------------------------------------
// ----------------------------------------------------------

CFiruTest::CFiruTest() :
    iCorrectTranslationIndex( -1 ), iRandSeed( 100 )
{
}

// ----------------------------------------------------------

CFiruTest::~CFiruTest()
{
    delete iEntry;
    iVariants.ResetAndDestroy();
}

// ----------------------------------------------------------

CFiruTest* CFiruTest::NewLC(
    CFiruDataEntry* aEntry,
    TBool aReversed )
{
    ASSERT( aEntry );

    CFiruTest* self = new (ELeave) CFiruTest;
    CleanupStack::PushL( self );

    self->iEntry = aEntry;
    self->iReversed = aReversed;

    HBufC* text = aEntry->Translation( 0 ).AllocLC();
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

    HBufC* correct = iVariants[ iCorrectTranslationIndex ];

    HBufC* text = aText.AllocLC();
    iVariants[ iCorrectTranslationIndex ] = text;
    CleanupStack::Pop( text );

    TInt newIndex = ( iVariants.Count() + 1 ) * Math::FRand( gRandSeed );
    iVariants.InsertL( correct, newIndex );
    iCorrectTranslationIndex = newIndex;
}

// ----------------------------------------------------------

TPtrC CFiruTest::Question() const
{
    return iEntry->Text();
}

// ----------------------------------------------------------

TPtrC CFiruTest::Answer() const
{
    return iEntry->Translation( 0 );
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
