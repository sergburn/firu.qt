#include "translationquery.h"
#include "database.h"

// ----------------------------------------------------------------------------

TranslationsQuery::TranslationsQuery( Database* db, LangPair langs, QObject* parent )
    : Query( db, langs, parent )
{
    m_tableName = Database::getTransTableName( langs );
}

// ----------------------------------------------------------------------------

void TranslationsQuery::setSort( Sort sort, bool ascending )
{
    m_sort = sort;
    m_sortAscending = ascending;
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

TranslationByIdQuery::TranslationByIdQuery( Database* db, LangPair langs, QObject* parent )
    : TranslationsQuery( db, langs, parent )
{
}

// ----------------------------------------------------------------------------

QString TranslationByIdQuery::buildSql() const
{
    QString sql = selectBaseSql();
    addPrimaryKeyCondition( sql );
    return sql;
}

// ----------------------------------------------------------------------------

int TranslationByIdQuery::bind()
{
    return bindPrimaryKey();
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

TranslationsBySidQuery::TranslationsBySidQuery( Database* db, LangPair langs, QObject* parent )
    : TranslationsQuery( db, langs, parent )
{
}

// ----------------------------------------------------------------------------

QString TranslationsBySidQuery::buildSql() const
{
    QString sql = selectBaseSql();
    addCondition( sql, "sid = :sid" );
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

void TranslationsByPatternQuery::setPattern( const QString& pattern, TextMatch match )
{
    m_pattern = pattern;
    m_match = match;
}

// ----------------------------------------------------------------------------

QString TranslationsByPatternQuery::buildSql() const
{
    QString sql = selectBaseSql();
    addCondition( sql, "text LIKE :pattern" );
}

// ----------------------------------------------------------------------------

int TranslationsByPatternQuery::bind()
{
    QString pattern = createPattern( m_pattern, m_match );
    return bindString( ":pattern", pattern );
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

TranslationUpdateQuery::TranslationUpdateQuery( Database* db, LangPair langs, QObject* parent )
    : TranslationsQuery( db, langs, parent )
{
}

// ----------------------------------------------------------------------------

QString TranslationUpdateQuery::buildSql() const
{
    QString sql = updateBaseSql();
    addPrimaryKeyCondition( sql );
    addSet( sql, "text = :text");
    addSet( sql, "sid = :sid");
    addSet( sql, "fmark = :fmark");
    addSet( sql, "rmark = :rmark");
    return sql;
}

// ----------------------------------------------------------------------------

int TranslationUpdateQuery::bind()
{
    int err = bindPrimaryKey();
    if ( !err ) err = bindInt64( ":sid", m_record.text );
    if ( !err ) err = bindString( ":text", m_record.text );
    if ( !err ) err = bindInt( ":fmark", m_record.fmark );
    if ( !err ) err = bindString( ":rmark", m_record.rmark );
    return err;
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

TranslationInsertQuery::TranslationInsertQuery( Database* db, LangPair langs, QObject* parent )
    : TranslationsQuery( db, langs, parent )
{
}

// ----------------------------------------------------------------------------

QString TranslationInsertQuery::buildSql() const
{
    return QString( "INSERT INTO %1 ( sid, text, fmark, rmark ) VALUES ( :sid, :text, :fmark, : rmark )" ).arg( m_tableName );
}

// ----------------------------------------------------------------------------

int TranslationInsertQuery::execute()
{
    int err = TranslationsQuery::execute();
    if ( !err )
    {
        m_record.id = sqlite3_last_insert_rowid( m_db );
    }
    return err;
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

UpdateMarksQuery::UpdateMarksQuery( Database* db, LangPair langs, QObject* parent )
:   TranslationsQuery( db, langs, parent ),
    m_fMarkValue( Undefined ),
    m_rMarkValue( Undefined )
{
}

// ----------------------------------------------------------------------------

void UpdateMarkQuery::resetMarks()
{
    m_fMarkValue = Mark::Undefined;
    m_rMarkValue = Mark::Undefined;
}

// ----------------------------------------------------------------------------

QString UpdateMarksQuery::buildSql() const
{
    QString sql = updateBaseSql();
    addSet( sql, "fmark = :fmark");
    addSet( sql, "rmark = :rmark");
    addCondition( sql, "sid = :sid");
    return sql;
}

// ----------------------------------------------------------------------------

int UpdateMarksQuery::bind()
{
    int err = bindInt64( ":sid", m_record.text );
    if ( !err ) err = bindInt( ":fmark", m_record.fmark );
    if ( !err ) err = bindString( ":rmark", m_record.rmark );
    return err;
}

// ----------------------------------------------------------------------------

int UpdateMarkQuery::prepare()
{
    const char* MarkUpdate = "UPDATE %1 SET %2 = :mark ";
    sql = QString( MarkUpdate ).arg( DbSchema::getTransTableName( m_srcLang, m_trgLang ) );

    if ( filterId ) addCondition( "id = :id" );
    if ( filterSid ) addCondition( "sid = :sid" );
    if ( filterFmark >= 0 ) addCondition( "fmark = :fmark" );
    if ( filterRmark >= 0 ) addCondition( "rmark = :rmark" );

    if ( m_fMarkValue > Mark::Undefined )
    {
        addSet( "fmark = :fmark" );
    }

    if ( m_rMarkValue > Mark::Undefined )
    {
        addSet( "rmark = :rmark" );
    }

    return sqlite3_prepare16_v2( m_db, sql.utf16(), -1, &m_stmt, NULL );
}
