/*
 * FiruDbSchema.cpp
 *
 *  Created on: 24.08.2008
 *      Author: sergey
 */

#include "FiruDbSchema.h"

// ----------------------------------------------------------

TBool FiruDbSchema::TableExistsL( RDbNamedDatabase& aDb, const TDesC& aTable )
{
    CDbTableNames* tables = aDb.TableNamesL();
    CleanupStack::PushL( tables );

    TBool tableExists = EFalse;
    for ( int i = 0; i < tables->Count(); i++ )
    {
        if ( (*tables)[i].Compare( aTable ) == 0 )
        {
            tableExists = ETrue;
            break;
        }
    }
    CleanupStack::PopAndDestroy( tables );
    return tableExists;
}

// ----------------------------------------------------------

CDbColSet* FiruDbSchema::CreateEntriesTableColumnsLC()
{
    CDbColSet* cols = CDbColSet::NewLC();
    TDbCol id( KColId, EDbColUint32);
    id.iAttributes = TDbCol::EAutoIncrement | TDbCol::ENotNull;
    cols->AddL( id );
    cols->AddL( TDbCol( KColText, EDbColText16, KMaxTextColumnLength ) );
    return cols;
}

// ----------------------------------------------------------

CDbColSet* FiruDbSchema::CreateTranslationsTableColumnsLC()
{
    CDbColSet* cols = CDbColSet::NewLC();
    TDbCol id( KColId, EDbColUint32);
    id.iAttributes = TDbCol::EAutoIncrement | TDbCol::ENotNull;
    cols->AddL( id );
    cols->AddL( TDbCol( KColSourceFk, EDbColUint32 ) );
    cols->AddL( TDbCol( KColTargetFk, EDbColUint32 ) );
    cols->AddL( TDbCol( KColForward, EDbColInt16 ) );
    cols->AddL( TDbCol( KColReverse, EDbColInt16 ) );
    cols->AddL( TDbCol( KColFwdCounter, EDbColInt16 ) );
    cols->AddL( TDbCol( KColRevCounter, EDbColInt16 ) );
    return cols;
}

// ----------------------------------------------------------

//CDbColSet* FiruDbSchema::CreateExamplesTableColumnsLC()
//{
//    CDbColSet* cols = CDbColSet::NewLC();
//    TDbCol id( KColId, EDbColUint32);
//    id.iAttributes = TDbCol::EAutoIncrement | TDbCol::ENotNull;
//    cols->AddL( id );
//    cols->AddL( TDbCol( KColEntryFk, EDbColUint32 ) );
//    cols->AddL( TDbCol( KColTranslationFk, EDbColUint32 ) );
//    cols->AddL( TDbCol( KColExample, EDbColText16, KMaxTextColumnLength ) );
//    return cols;
//}

// ----------------------------------------------------------

HBufC* FiruDbSchema::EntriesTableNameLC( TLanguage aInputLanguage )
{
    HBufC* name = HBufC::NewLC( KEntryTableNameFmt().Length() + KIntStringMaxLength );
    TPtr buf = name->Des();
    buf.Format( KEntryTableNameFmt(), aInputLanguage );
    return name;
}

// ----------------------------------------------------------

HBufC* FiruDbSchema::TranslationsTableNameLC( TLanguage aInputLanguage, TLanguage aOutputLanguage )
{
    HBufC* name = HBufC::NewLC( KTranslationTableNameFmt().Length() + 2 * KIntStringMaxLength );
    TPtr buf = name->Des();
    buf.Format( KTranslationTableNameFmt(), aInputLanguage, aOutputLanguage );
    return name;
}

// ----------------------------------------------------------

//HBufC* FiruDbSchema::ExamplesTableNameLC( TLanguage aInputLanguage, TLanguage aOutputLanguage )
//{
//    HBufC* name = HBufC::NewLC( KExampleTableNameFmt().Length() + 2 * KIntStringMaxLength );
//    TPtr buf = name->Des();
//    buf.Format( KExampleTableNameFmt(), aInputLanguage, aOutputLanguage );
//    return name;
//}

// ----------------------------------------------------------

void FiruDbSchema::CreateDictionaryL(
    RDbNamedDatabase& aDb,
    TLanguage aInputLanguage, TLanguage aOutputLanguage )
{
    HBufC* tableNameSources = EntriesTableNameLC( aInputLanguage );
    HBufC* tableNameTargets = EntriesTableNameLC( aOutputLanguage );
    HBufC* tableNameTranslations = TranslationsTableNameLC( aInputLanguage, aOutputLanguage );

    CDbKey* key = CDbKey::NewLC();

    if ( !TableExistsL( aDb, *tableNameSources ) )
    {
        CDbColSet* cols = CreateEntriesTableColumnsLC();
        User::LeaveIfError( aDb.CreateTable( *tableNameSources, *cols ) );
        CleanupStack::PopAndDestroy( cols );

        // ID index
        key->Clear();
        key->AddL( TDbKeyCol( KColId ) );
        User::LeaveIfError( aDb.CreateIndex( KPrimaryKeyIndexName(), *tableNameSources, *key ) );

        // Text index
        key->Clear();
        key->AddL( TDbKeyCol( KColText, KMaxTextColIndexLength ) );
        User::LeaveIfError( aDb.CreateIndex( KTextIndexName(), *tableNameSources, *key ) );
    }

    if ( !TableExistsL( aDb, *tableNameTargets ) )
    {
        CDbColSet* cols = CreateEntriesTableColumnsLC();
        User::LeaveIfError( aDb.CreateTable( *tableNameTargets, *cols ) );
        CleanupStack::PopAndDestroy( cols );

        // ID index
        key->Clear();
        key->AddL( TDbKeyCol( KColId ) );
        User::LeaveIfError( aDb.CreateIndex( KPrimaryKeyIndexName(), *tableNameTargets, *key ) );

        // Text index
        key->Clear();
        key->AddL( TDbKeyCol( KColText, KMaxTextColIndexLength ) );
        User::LeaveIfError( aDb.CreateIndex( KTextIndexName(), *tableNameTargets, *key ) );
    }

    if ( !TableExistsL( aDb, *tableNameTranslations ) )
    {
        CDbColSet* cols = CreateTranslationsTableColumnsLC();
        User::LeaveIfError( aDb.CreateTable( *tableNameTranslations, *cols ) );
        CleanupStack::PopAndDestroy( cols );

        // ID index
        key->Clear();
        key->AddL( TDbKeyCol( KColId ) );
        User::LeaveIfError( aDb.CreateIndex( KPrimaryKeyIndexName(), *tableNameTranslations, *key ) );

        // Source FK index
        key->Clear();
        key->AddL( TDbKeyCol( KColSourceFk ) );
        User::LeaveIfError( aDb.CreateIndex( KSourceFkIndexName(), *tableNameTranslations, *key ) );

        // Target FK index
        key->Clear();
        key->AddL( TDbKeyCol( KColTargetFk ) );
        User::LeaveIfError( aDb.CreateIndex( KTargetFkIndexName(), *tableNameTranslations, *key ) );
    }

    CleanupStack::PopAndDestroy( key );

    CleanupStack::PopAndDestroy( tableNameTranslations );
    CleanupStack::PopAndDestroy( tableNameTargets );
    CleanupStack::PopAndDestroy( tableNameSources );
}

// ----------------------------------------------------------

TInt FiruDbSchema::AddEntryL( RDbRowSet& aTable, const TDesC& aEntry )
{
    aTable.InsertL();
    aTable.SetColL( KColumnText, aEntry.Left( KMaxTextColumnLength ) );
    aTable.PutL();
    return aTable.ColUint32( KColumnId );
}

// ----------------------------------------------------------

TInt FiruDbSchema::AddTranslationL(
    RDbRowSet& aTable,
    TInt aSourceEntryId,
    TInt aTargetEntryId )
{
    aTable.InsertL();
    aTable.SetColL( KColumnSourceFk, aSourceEntryId );
    aTable.SetColL( KColumnTargetFk, aTargetEntryId );
    aTable.SetColL( KColumnForwardMark, 0 );
    aTable.SetColL( KColumnForwardCount, 0 );
    aTable.SetColL( KColumnReverseMark, 0 );
    aTable.SetColL( KColumnReverseCount, 0 );
    aTable.PutL();
    return aTable.ColUint32( KColumnId );
}

// ----------------------------------------------------------

//TInt FiruDbSchema::AddExampleL( RDbRowSet& aTable, TInt aEntryId, TInt aTranslationId, const TDesC& aExample )
//{
//    aTable.InsertL();
//    aTable.SetColL( KColumnEntryFk, aEntryId );
//    aTable.SetColL( KColumnExampleTranslationFk, aTranslationId );
//    aTable.SetColL( KColumnExampleText, aExample.Left( KMaxTextColumnLength ) );
//    aTable.PutL();
//    return aTable.ColUint32( KColumnId );
//}

