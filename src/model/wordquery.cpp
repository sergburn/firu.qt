#include "wordquery.h"

WordsQuery::WordsQuery( sqlite* db, Lang src, QObject* parent = NULL )
    : Query( db, src, QLocale::C, parent )
{
    m_tableName = DbSchema::getWordTableName( src );
    QString sql = selectBaseSql( m_tableName );
    return sqlite3_prepare16_v2( m_db, sql.utf16(), -1, &m_stmt, NULL );
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

QString WordsQuery::selectBaseSql( const char* tableName ) const
{
    return QString( "SELECT * FROM %1" ).arg(  );
}

// ----------------------------------------------------------------------------

QString WordsQuery::deleteBaseSql( const char* tableName ) const
{
    return QString( "DELETE FROM %1" ).arg( DbSchema::getWordTableName( m_srcLang ) );
}

// ----------------------------------------------------------------------------

QString WordsQuery::updateBaseSql() const
{
    return QString( "UPDATE %1 SET text = :text" ).arg( DbSchema::getWordTableName( m_srcLang ) );
}

// ----------------------------------------------------------------------------

QString WordsQuery::insertBaseSql() const
{
    return QString( "INSERT INTO %1 ( text ) VALUES ( :text )" ).arg( DbSchema::getWordTableName( m_srcLang ) );
}

// ----------------------------------------------------------------------------

WordByIdQuery::WordByIdQuery( sqlite* db, Lang src, QObject* parent )
    : WordsQuery( db, src, parent )
{
    QString sql = selectBaseSql( m_tableName );
    addCondition( sql, "e.id = :id" );
    sqlite3_prepare16_v2( m_db, sql.utf16(), -1, &m_stmt, NULL );
}

// ----------------------------------------------------------------------------

void WordByIdQuery::setPattern( qint64 id )
{
    m_id = id;
}

// ----------------------------------------------------------------------------

int WordByIdQuery::bind()
{
    int err = bindInt64( ":id", m_id );
    LogIfSqlError( err, "WordByIdQuery::bind()" );
    return err;
}

// ----------------------------------------------------------------------------

WordsByPatternQuery::WordsByPatternQuery( sqlite* db, Lang src, QObject* parent )
    : WordsQuery( db, src, parent )
{
    QString sql = selectBaseSql();
    addCondition( sql, "e.text LIKE :pattern" );
    sqlite3_prepare16_v2( m_db, sql.utf16(), -1, &m_stmt, NULL );
}

// ----------------------------------------------------------------------------

void WordsByPatternQuery::setPattern( const QString& pattern, TextMatch match )
{
    m_pattern = pattern;
    m_match = match;
}

// ----------------------------------------------------------------------------

int WordsByPatternQuery::bind()
{
    QString pattern = createPattern( m_pattern, m_match );
    int err = bindString( ":pattern", pattern );
    LogIfSqlError( err, "WordsQuery::bind()" );
    return err;
}
