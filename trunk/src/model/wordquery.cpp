#include "wordquery.h"

// ----------------------------------------------------------------------------

WordsQuery::WordsQuery( sqlite* db, Lang src, QObject* parent = NULL )
    : Query( db, src, parent )
{
    m_tableName = DbSchema::getWordTableName( src );
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
    record.id = sqlite3_column_int64( m_stmt, 0 );
    record.text = QString::fromUtf8( (const char*) sqlite3_column_text( m_stmt, 1 ) );
}

// ----------------------------------------------------------------------------

QString WordsQuery::buildSql() const
{
    return selectBaseSql();
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

WordByIdQuery::WordByIdQuery( sqlite* db, Lang src, QObject* parent )
    : WordsQuery( db, src, parent )
{
}

// ----------------------------------------------------------------------------

QString WordByIdQuery::buildSql() const
{
    QString sql = selectBaseSql();
    addPrimaryKeyCondition( sql );
}

// ----------------------------------------------------------------------------

int WordByIdQuery::bind()
{
    return bindPrimaryKey();
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

WordsByPatternQuery::WordsByPatternQuery( sqlite* db, Lang src, QObject* parent )
    : WordsQuery( db, src, parent )
{
}

// ----------------------------------------------------------------------------

void WordsByPatternQuery::setPattern( const QString& pattern, TextMatch match )
{
    m_pattern = pattern;
    m_match = match;
}

// ----------------------------------------------------------------------------

QString WordsByPatternQuery::buildSql() const
{
    QString sql = selectBaseSql();
    addCondition( sql, "text LIKE :pattern" );
}

// ----------------------------------------------------------------------------

int WordsByPatternQuery::bind()
{
    QString pattern = createPattern( m_pattern, m_match );
    return bindString( ":pattern", pattern );
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

WordUpdateQuery::WordUpdateQuery( sqlite* db, Lang src, QObject* parent )
    : WordsQuery( db, src, parent )
{
}

// ----------------------------------------------------------------------------

QString WordUpdateQuery::buildSql() const
{
    QString sql = updateBaseSql();
    addSet( sql, "text = :text");
    return sql;
}

// ----------------------------------------------------------------------------

int WordUpdateQuery::bind()
{
    return bindString( ":text", m_record.text );
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

WordInsertQuery::WordInsertQuery( sqlite* db, Lang src, QObject* parent )
    : WordsQuery( db, src, parent )
{
}

// ----------------------------------------------------------------------------

QString WordInsertQuery::buildSql() const
{
    return QString( "INSERT INTO %1 ( text ) VALUES ( :text )" ).arg( m_tableName );
}

// ----------------------------------------------------------------------------

int WordInsertQuery::execute()
{
    int err = WordsQuery::execute();
    if ( !err )
    {
        m_record.id = sqlite3_last_insert_rowid( m_db );
    }
    return err;
}
