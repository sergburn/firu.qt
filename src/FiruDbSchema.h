/*
 * FiruDbSchema.h
 *
 *  Created on: 24.08.2008
 *      Author: sergey
 */

#ifndef FIRUDBSCHEMA_H_
#define FIRUDBSCHEMA_H_

#include <e32std.h>
#include <d32dbms.h>

_LIT( KDatabaseFile, "c:\\private\\E0EDB1C1\\dicts.db" );
_LIT( KEntryTableNameFmt, "entries_%02d" );
_LIT( KTranslationTableNameFmt, "trans_%02d_to_%02d" );
_LIT( KExampleTableNameFmt, "ex_%02d_to_%02d" );
_LIT( KDirTableIdIndexNameFmt, "ind_%S_id" );
_LIT( KDirTableSrcIndexNameFmt, "ind_%S_src" );
_LIT( KDirTableTrgIndexNameFmt, "ind_%S_trg" );

_LIT( KPrimaryKeyIndexName, "index_PK" );
_LIT( KEntryFKIndexName, "index_EntryFK" );
_LIT( KTranslationFKIndexName, "index_TranslationFK" );
_LIT( KSourceTextIndexName, "index_source" );
_LIT( KTargetTextIndexName, "index_target" );

_LIT( KSqlViewAll, "SELECT * FROM %S");
_LIT( KSqlViewWhere, "SELECT * FROM %S WHERE ");
_LIT( KSqlDeleteAll, "DELETE FROM %S");
_LIT( KSqlLike, " %S LIKE '%S' ");
_LIT( KSqlLessEqual, " %S <= %d ");
_LIT( KSqlEqual, " %S = %d ");
_LIT( KSqlOrder, " ORDER BY %S");
_LIT( KWildCardStar, "*" );

const TInt KColumnId = 1;

// Entries table
// id - record id, int32, autoinc, PK
// source - source text, unicode

enum {
    KColumnEntryPK = KColumnId,
    KColumnSource,
};

// Translations table
// id - record id, int32, autoinc, PK
// eid - parent entry id, FK
// target - target text, unicode
// forward - from source to target association mark, int
// reverse - from target to source association mark, int
// fwd_count - amount of times source was used in tests, int
// rev_count - amount of times target was used in tests, int

enum {
    KColumnTransPK = KColumnId,
    KColumnEntryFk,
    KColumnTarget,
    KColumnForwardMark,
    KColumnReverseMark,
    KColumnForwardCount,
    KColumnReverseCount,
};

// Examples table
// id - record id, int32, autoinc, PK
// eid - parent entry record, FK
// tid - parent translation record, FK
// example - example text

enum {
    KColumnExamplePK = KColumnId,
    KColumnExampleEntryFk,
    KColumnExampleTranslationFk,
    KColumnExampleText,
};

_LIT( KColId, "id" );
_LIT( KColEntryFk, "eid" );
_LIT( KColTranslationFk, "tid" );
_LIT( KColSource, "source" );
_LIT( KColTarget, "target" );
_LIT( KColExample, "example" );
_LIT( KColForward, "forward" );
_LIT( KColReverse, "reverse" );
_LIT( KColFwdCounter, "fwd_count" );
_LIT( KColRevCounter, "rev_count" );

const TInt KMaxTextColumnLength = 250;
const TInt KMaxTextColIndexLength = 16;

const TInt KIntStringMaxLength = 11;

class FiruDbSchema
{
public:

    static HBufC* EntriesTableNameLC( TLanguage aInputLanguage );
    static HBufC* TranslationsTableNameLC( TLanguage aInputLanguage, TLanguage aOutputLanguage );
    static HBufC* ExamplesTableNameLC( TLanguage aInputLanguage, TLanguage aOutputLanguage );

    static HBufC* ForwardIndexNameLC( const TDesC& aTableName );
    static HBufC* ReverseIndexNameLC( const TDesC& aTableName );

    static CDbColSet* CreateEntriesTableColumnsLC();
    static CDbColSet* CreateTranslationsTableColumnsLC();
    static CDbColSet* CreateExamplesTableColumnsLC();

    static void CreateDictionaryL(
        RDbNamedDatabase& aDb,
        TLanguage aInputLanguage, TLanguage aOutputLanguage );

    static TInt AddEntryL( RDbRowSet& aTable, const TDesC& aEntry );
    static TInt AddTranslationL( RDbRowSet& aTable, TInt aEntryId, const TDesC& aTranslation );
    static TInt AddExampleL( RDbRowSet& aTable, TInt aEntryId, TInt aTranslationId, const TDesC& aExample );

private:
    static TBool TableExistsL( RDbNamedDatabase& aDb, const TDesC& aTable );
};

#endif /* FIRUDBSCHEMA_H_ */
