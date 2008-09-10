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
    cols->AddL( TDbCol( KColSource, EDbColText16, KMaxTextColumnLength ) );
    return cols;
}

// ----------------------------------------------------------

CDbColSet* FiruDbSchema::CreateTranslationsTableColumnsLC()
{
    CDbColSet* cols = CDbColSet::NewLC();
    TDbCol id( KColId, EDbColUint32);
    id.iAttributes = TDbCol::EAutoIncrement | TDbCol::ENotNull;
    cols->AddL( id );
    cols->AddL( TDbCol( KColEntryFk, EDbColUint32 ) );
    cols->AddL( TDbCol( KColTarget, EDbColText16, KMaxTextColumnLength ) );
    cols->AddL( TDbCol( KColForward, EDbColInt16 ) );
    cols->AddL( TDbCol( KColReverse, EDbColInt16 ) );
    cols->AddL( TDbCol( KColFwdCounter, EDbColInt16 ) );
    cols->AddL( TDbCol( KColRevCounter, EDbColInt16 ) );
    return cols;
}

// ----------------------------------------------------------

CDbColSet* FiruDbSchema::CreateExamplesTableColumnsLC()
{
    CDbColSet* cols = CDbColSet::NewLC();
    TDbCol id( KColId, EDbColUint32);
    id.iAttributes = TDbCol::EAutoIncrement | TDbCol::ENotNull;
    cols->AddL( id );
    cols->AddL( TDbCol( KColEntryFk, EDbColUint32 ) );
    cols->AddL( TDbCol( KColTranslationFk, EDbColUint32 ) );
    cols->AddL( TDbCol( KColExample, EDbColText16, KMaxTextColumnLength ) );
    return cols;
}

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

HBufC* FiruDbSchema::ExamplesTableNameLC( TLanguage aInputLanguage, TLanguage aOutputLanguage )
{
    HBufC* name = HBufC::NewLC( KExampleTableNameFmt().Length() + 2 * KIntStringMaxLength );
    TPtr buf = name->Des();
    buf.Format( KExampleTableNameFmt(), aInputLanguage, aOutputLanguage );
    return name;
}

// ----------------------------------------------------------

HBufC* FiruDbSchema::ForwardIndexNameLC( const TDesC& aTableName )
{
    HBufC* name = HBufC::NewLC( KDirTableSrcIndexNameFmt().Length() + 2 * KIntStringMaxLength );
    TPtr buf = name->Des();
    buf.Format( KDirTableSrcIndexNameFmt, &aTableName );
    return name;
}

// ----------------------------------------------------------

HBufC* FiruDbSchema::ReverseIndexNameLC( const TDesC& aTableName )
{
    HBufC* name = HBufC::NewLC( KDirTableTrgIndexNameFmt().Length() + 2 * KIntStringMaxLength );
    TPtr buf = name->Des();
    buf.Format( KDirTableTrgIndexNameFmt, &aTableName );
    return name;
}

// ----------------------------------------------------------

void FiruDbSchema::CreateDictionaryL(
    RDbNamedDatabase& aDb,
    TLanguage aInputLanguage, TLanguage aOutputLanguage )
{
    HBufC* tableNameEntries = EntriesTableNameLC( aInputLanguage );
    HBufC* tableNameTranslations = TranslationsTableNameLC( aInputLanguage, aOutputLanguage );
    HBufC* tableNameExamples = ExamplesTableNameLC( aInputLanguage, aOutputLanguage );

    CDbKey* key = CDbKey::NewLC();

    if ( !TableExistsL( aDb, *tableNameEntries ) )
    {
        CDbColSet* cols = CreateEntriesTableColumnsLC();
        User::LeaveIfError( aDb.CreateTable( *tableNameEntries, *cols ) );
        CleanupStack::PopAndDestroy( cols );

        // ID index
        key->Clear();
        key->AddL( TDbKeyCol( KColId ) );
        User::LeaveIfError( aDb.CreateIndex( KPrimaryKeyIndexName(), *tableNameEntries, *key ) );

        // Source text index
        key->Clear();
        key->AddL( TDbKeyCol( KColSource, KMaxTextColIndexLength ) );
        User::LeaveIfError( aDb.CreateIndex( KSourceTextIndexName(), *tableNameEntries, *key ) );
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

        // Entry FK index
        key->Clear();
        key->AddL( TDbKeyCol( KColEntryFk ) );
        User::LeaveIfError( aDb.CreateIndex( KEntryFKIndexName(), *tableNameTranslations, *key ) );

        // Target text index
        key->Clear();
        key->AddL( TDbKeyCol( KColTarget, KMaxTextColIndexLength ) );
        User::LeaveIfError( aDb.CreateIndex( KTargetTextIndexName(), *tableNameTranslations, *key ) );
    }

    if ( !TableExistsL( aDb, *tableNameExamples ) )
    {
        CDbColSet* cols = CreateExamplesTableColumnsLC();
        User::LeaveIfError( aDb.CreateTable( *tableNameExamples, *cols ) );
        CleanupStack::PopAndDestroy( cols );

        // ID index
        key->Clear();
        key->AddL( TDbKeyCol( KColId ) );
        User::LeaveIfError( aDb.CreateIndex( KPrimaryKeyIndexName(), *tableNameExamples, *key ) );

        // Entry FK index
        key->Clear();
        key->AddL( TDbKeyCol( KColEntryFk ) );
        User::LeaveIfError( aDb.CreateIndex( KEntryFKIndexName(), *tableNameExamples, *key ) );

        // Translation FK index
        key->Clear();
        key->AddL( TDbKeyCol( KColTranslationFk ) );
        User::LeaveIfError( aDb.CreateIndex( KTranslationFKIndexName(), *tableNameExamples, *key ) );
    }

    CleanupStack::PopAndDestroy( key );
}

// ----------------------------------------------------------

TInt FiruDbSchema::AddEntryL( RDbRowSet& aTable, const TDesC& aEntry )
{
    aTable.InsertL();
    aTable.SetColL( KColumnSource, aEntry.Left( KMaxTextColumnLength ) );
    aTable.PutL();
    return aTable.ColUint32( KColumnId );
}

// ----------------------------------------------------------

TInt FiruDbSchema::AddTranslationL( RDbRowSet& aTable, TInt aEntryId, const TDesC& aTranslation )
{
    aTable.InsertL();
    aTable.SetColL( KColumnEntryFk, aEntryId );
    aTable.SetColL( KColumnTarget, aTranslation.Left( KMaxTextColumnLength ) );
    aTable.SetColL( KColumnForwardMark, 0 );
    aTable.SetColL( KColumnForwardCount, 0 );
    aTable.SetColL( KColumnReverseMark, 0 );
    aTable.SetColL( KColumnReverseCount, 0 );
    aTable.PutL();
    return aTable.ColUint32( KColumnId );
}

// ----------------------------------------------------------

TInt FiruDbSchema::AddExampleL( RDbRowSet& aTable, TInt aEntryId, TInt aTranslationId, const TDesC& aExample )
{
    aTable.InsertL();
    aTable.SetColL( KColumnEntryFk, aEntryId );
    aTable.SetColL( KColumnExampleTranslationFk, aTranslationId );
    aTable.SetColL( KColumnExampleText, aExample.Left( KMaxTextColumnLength ) );
    aTable.PutL();
    return aTable.ColUint32( KColumnId );
}

