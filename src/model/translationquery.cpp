#include "translationquery.h"

// ----------------------------------------------------------------------------

TranslationQuery::TranslationQuery( sqlite3* db, Lang src, Lang trg, QObject* parent = NULL )
    : Query( db, src, trg, parent )
{
    m_tableName = DbSchema::getTransTableName( src, trg );
}

// ----------------------------------------------------------------------------

void TranslationQuery::setSort( Sort sort, bool ascending = true )
{
    m_sort = sort;
    m_sortAscending = ascending;
}

// ----------------------------------------------------------------------------

const TranslationQuery::Record& TranslationQuery::record() const
{
    return m_record;
}

// ----------------------------------------------------------------------------

TranslationQuery::Record& TranslationQuery::record()
{
    return m_record;
}

// ----------------------------------------------------------------------------

void TranslationQuery::read()
{
    record.id = sqlite3_column_int64( m_stmt, 0 );
    record.sid = sqlite3_column_int64( m_stmt, 1 );
    record.text = QString::fromUtf8( (const char*) sqlite3_column_text( m_stmt, 2 ) );
    record.fmark = sqlite3_column_int( m_stmt, 3 );
    record.rmark = sqlite3_column_int( m_stmt, 4 );
}

// ----------------------------------------------------------------------------

QString TranslationQuery::buildSql() const
{
    return selectBaseSql();
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

TranslationByIdQuery::TranslationByIdQuery( sqlite* db, Lang src, Lang trg, QObject* parent )
    : TranslationsQuery( db, src, trg, parent )
{
}

// ----------------------------------------------------------------------------

QString TranslationByIdQuery::buildSql() const
{
    QString sql = selectBaseSql();
    addPrimaryKeyCondition( sql );
}

// ----------------------------------------------------------------------------

int TranslationByIdQuery::bind()
{
    return bindPrimaryKey();
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

TranslationBySidQuery::TranslationBySidQuery( sqlite* db, Lang src, Lang trg, QObject* parent )
    : TranslationsQuery( db, src, trg, parent )
{
}

// ----------------------------------------------------------------------------

QString TranslationBySidQuery::buildSql() const
{
    QString sql = selectBaseSql();
    addCondition( sql, "sid = :sid" );
}

// ----------------------------------------------------------------------------

void TranslationBySidQuery::setSourceEntry( qint64 id )
{
    m_sid = id;
}

// ----------------------------------------------------------------------------

int TranslationBySidQuery::bind()
{
    return bindInt64( ":sid", m_sid );
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

TranslationUpdateQuery::TranslationUpdateQuery( sqlite* db, Lang src, QObject* parent )
    : TranslationsQuery( db, src, parent )
{
}

// ----------------------------------------------------------------------------

QString TranslationUpdateQuery::buildSql() const
{
    QString sql = updateBaseSql();
    addSet( sql, "text = :text");
    addSet( sql, "sid = :sid");
    addSet( sql, "fmark = :fmark");
    addSet( sql, "rmark = :rmark");
    return sql;
}

// ----------------------------------------------------------------------------

int TranslationUpdateQuery::bind()
{
    int err = bindInt64( ":sid", m_record.text );
    if ( !err ) err = bindString( ":text", m_record.text );
    if ( !err ) err = bindInt( ":fmark", m_record.fmark );
    if ( !err ) err = bindString( ":rmark", m_record.rmark );
    return err;
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

TranslationInsertQuery::TranslationInsertQuery( sqlite* db, Lang src, QObject* parent )
    : TranslationsQuery( db, src, parent )
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

UpdateMarkQuery::UpdateMarkQuery( sqlite3* db, Lang src, Lang trg )
    : TranslationQuery( db, src, trg )
{
    resetMarks();
}

// ----------------------------------------------------------------------------

void UpdateMarkQuery::resetMarks()
{
    m_fMarkValue = Mark::Undefined;
    m_rMarkValue = Mark::Undefined;
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
