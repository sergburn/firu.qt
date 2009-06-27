#include "wordquery.h"
#include "database.h"
#include "sqlgenerator.h"
#include "../firudebug.h"

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

WordsQuery::WordsQuery( Database* db, LangPair langs )
    : Query( db )
{
    m_tableName = Database::getWordTableName( langs.first );
}

// ----------------------------------------------------------------------------

WordsQuery::Record& WordsQuery::record()
{
    return m_record;
}

// ----------------------------------------------------------------------------

const WordsQuery::Record& WordsQuery::record() const
{
    return m_record;
}

// ----------------------------------------------------------------------------

void WordsQuery::read()
{
    m_record.id = sqlite3_column_int64( m_stmt, 0 );
    m_record.text = QString::fromUtf8( (const char*) sqlite3_column_text( m_stmt, 1 ) );
}

// ----------------------------------------------------------------------------

QString WordsQuery::buildSql() const
{
    return selectBaseSql();
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

QString WordsCountQuery::buildSql() const
{
    return countBaseSql();
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

QString WordByIdQuery::buildSql() const
{
    SqlGenerator builder( selectBaseSql() );
    builder.addPrimaryKeyCondition();
    return builder.sql();
}

// ----------------------------------------------------------------------------

int WordByIdQuery::bind()
{
    return bindPrimaryKey();
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

void WordsByPatternQuery::setPattern( const QString& pattern, TextMatch match )
{
    m_pattern = pattern;
    m_match = match;
}

// ----------------------------------------------------------------------------

QString WordsByPatternQuery::buildSql() const
{
    SqlGenerator builder( selectBaseSql() );
    builder.addCondition( "text LIKE :pattern" );
    return builder.sql();
}

// ----------------------------------------------------------------------------

int WordsByPatternQuery::bind()
{
    QString pattern = SqlGenerator::createPattern( m_pattern, m_match );
    return bindString( ":pattern", pattern );
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

QString WordUpdateQuery::buildSql() const
{
    SqlGenerator builder( updateBaseSql() );
    builder.addPrimaryKeyCondition();
    builder.addSet( "text = :text");
    builder.addSet( "modified = julianday('now')" );
    return builder.sql();
}

// ----------------------------------------------------------------------------

int WordUpdateQuery::bind()
{
    int err = bindPrimaryKey();
    if ( !err ) err = bindString( ":text", m_record.text );
    return err;
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

QString WordInsertQuery::buildSql() const
{
    return QString( "INSERT INTO %1 ( text ) VALUES ( :text )" ).arg( m_tableName );
}

// ----------------------------------------------------------------------------

bool WordInsertQuery::execute()
{
    int err = WordsQuery::execute();
    if ( !err )
    {
        m_record.id = sqlite3_last_insert_rowid( m_db );
    }
    return SQLOK( err ) == SQLITE_OK;
}
