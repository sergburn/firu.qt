#include "translationquery.h"
#include "database.h"
#include "sqlgenerator.h"
#include "../firudebug.h"

// ----------------------------------------------------------------------------

TranslationsQuery::TranslationsQuery( Database* db, LangPair langs )
    : Query( db )
{
    m_tableName = Database::getTransTableName( langs );
}

// ----------------------------------------------------------------------------

const TranslationsQuery::Record& TranslationsQuery::record() const
{
    return m_record;
}

// ----------------------------------------------------------------------------

TranslationsQuery::Record& TranslationsQuery::record()
{
    return m_record;
}

// ----------------------------------------------------------------------------

void TranslationsQuery::read()
{
    m_record.id = sqlite3_column_int64( m_stmt, 0 );
    m_record.sid = sqlite3_column_int64( m_stmt, 1 );
    m_record.text = QString::fromUtf8( (const char*) sqlite3_column_text( m_stmt, 2 ) );
    m_record.fmark = sqlite3_column_int( m_stmt, 3 );
    m_record.rmark = sqlite3_column_int( m_stmt, 4 );
}

// ----------------------------------------------------------------------------

QString TranslationsQuery::buildSql() const
{
    return selectBaseSql();
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

QString TranslationByIdQuery::buildSql() const
{
    SqlGenerator builder( selectBaseSql() );
    builder.addPrimaryKeyCondition();
    return builder.sql();
}

// ----------------------------------------------------------------------------

int TranslationByIdQuery::bind()
{
    return bindPrimaryKey();
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

QString TranslationsBySidQuery::buildSql() const
{
    SqlGenerator builder( selectBaseSql() );
    builder.addCondition( "sid = :sid" );
    return builder.sql();
}

// ----------------------------------------------------------------------------

void TranslationsBySidQuery::setSourceEntry( qint64 id )
{
    m_sid = id;
}

// ----------------------------------------------------------------------------

int TranslationsBySidQuery::bind()
{
    return bindInt64( ":sid", m_sid );
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

QString TranslationsByRMarkQuery::buildSql() const
{
    SqlGenerator builder( selectBaseSql() );
    builder.addCondition( "rmark = :rmark" );
    return builder.sql();
}

// ----------------------------------------------------------------------------

void TranslationsByRMarkQuery::setRequiredMark( Mark::MarkValue rmark )
{
    m_rmark = rmark;
}

// ----------------------------------------------------------------------------

int TranslationsByRMarkQuery::bind()
{
    return bindInt( ":rmark", m_rmark );
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

void TranslationsByPatternQuery::setPattern( const QString& pattern, TextMatch match )
{
    m_pattern = pattern;
    m_match = match;
}

// ----------------------------------------------------------------------------

QString TranslationsByPatternQuery::buildSql() const
{
    SqlGenerator builder( selectBaseSql() );
    builder.addCondition( "text LIKE :pattern" );
    return builder.sql();
}

// ----------------------------------------------------------------------------

int TranslationsByPatternQuery::bind()
{
    QString pattern = SqlGenerator::createPattern( m_pattern, m_match );
    return bindString( ":pattern", pattern );
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

QString TranslationUpdateQuery::buildSql() const
{
    SqlGenerator builder( updateBaseSql() );
    builder.addPrimaryKeyCondition();
    builder.addSet( "text = :text");
    builder.addSet( "sid = :sid");
    builder.addSet( "fmark = :fmark");
    builder.addSet( "rmark = :rmark");
    return builder.sql();
}

// ----------------------------------------------------------------------------

int TranslationUpdateQuery::bind()
{
    int err = bindPrimaryKey();
    if ( !err ) err = bindInt64( ":sid", m_record.sid );
    if ( !err ) err = bindString( ":text", m_record.text );
    if ( !err ) err = bindInt( ":fmark", m_record.fmark );
    if ( !err ) err = bindInt( ":rmark", m_record.rmark );
    return err;
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------

QString TranslationInsertQuery::buildSql() const
{
    return QString( "INSERT INTO %1 ( sid, text, fmark, rmark ) VALUES ( :sid, :text, :fmark, : rmark )" ).arg( m_tableName );
}

// ----------------------------------------------------------------------------

bool TranslationInsertQuery::execute()
{
    int err = TranslationsQuery::execute();
    if ( !err )
    {
        m_record.id = sqlite3_last_insert_rowid( m_db );
    }
    return SQLOK( err ) == SQLITE_OK;
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

UpdateMarksQuery::UpdateMarksQuery( Database* db, LangPair langs )
:   TranslationsQuery( db, langs )
{
}

// ----------------------------------------------------------------------------

QString UpdateMarksQuery::buildSql() const
{
    SqlGenerator builder( updateBaseSql() );
    builder.addSet( "fmark = :fmark" );
    builder.addSet( "rmark = :rmark" );
    builder.addCondition( "sid = :sid" );
    return builder.sql();
}

// ----------------------------------------------------------------------------

int UpdateMarksQuery::bind()
{
    int err = bindInt64( ":sid", m_record.sid );
    if ( !err ) err = bindInt( ":fmark", m_record.fmark );
    if ( !err ) err = bindInt( ":rmark", m_record.rmark );
    return err;
}
