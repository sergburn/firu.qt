#include "wordquery.h"

// ----------------------------------------------------------------------------

WordQuery::WordQuery( sqlite3* db, Lang src )
    : Query( db, src )
{
}

// ----------------------------------------------------------------------------

const WordQuery::Record& WordQuery::record() const
{
    return m_record;
}

// ----------------------------------------------------------------------------

void WordQuery::resetFilters()
{
    filterId = 0;
    filterPattern = 0;
}

// ----------------------------------------------------------------------------

void WordQuery::read()
{
    record.id = sqlite3_column_int64( m_stmt, 0 );
    record.text = QString::fromUtf8( (const char*) sqlite3_column_text( m_stmt, 1 ) );
}

// ----------------------------------------------------------------------------

int WordQuery::prepare( const char* tableName )
{
    const char* WordView = "SELECT e.id, e.text FROM %1 as e";
    sql = QString( SelectWordView ).arg( DbSchema::getWordTableName( m_srcLang ) );

    if ( filterId ) addCondition( "t.id = :id" );
    if ( filterPattern ) addCondition( "t.text LIKE :pattern" );

    return sqlite3_prepare16_v2( m_db, sql.utf16(), -1, &m_stmt, NULL );
}

// ----------------------------------------------------------------------------

int WordQuery::bind()
{
    int err = SQLITE_OK;
    if ( filterId ) err = bindInt64( ":id", filterId );
    if ( !err && filterPattern ) err = bindString( ":pattern", filterPattern );
    LogIfSqlError( err, "WordQuery::bind()" );
    return err;
}
