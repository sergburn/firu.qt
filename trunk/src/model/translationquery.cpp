#include "translationquery.h"

// ----------------------------------------------------------------------------

TranslationQuery::TranslationQuery( sqlite3* db, Lang src, Lang trg )
    : Query( db, src, trg )
{
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

void TranslationQuery::resetFilters()
{
    filterId = 0;
    filterSid = 0;
    filterFmark = -1;
    filterRmark = -1;
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

int TranslationQuery::prepare( const char* tableName )
{
    const char* TransView =
        "SELECT t.id, t.sid, t.text, t.fmark, t.rmark "
        "FROM %1 as t ";
    sql = QString( TransView ).arg( DbSchema::getTransTableName( m_srcLang, m_trgLang ) );

    if ( filterId ) addCondition( "t.id = :id" );
    if ( filterSid ) addCondition( "t.sid = :sid" );
    if ( filterFmark >= 0 ) addCondition( "t.fmark = :fmark" );
    if ( filterRmark >= 0 ) addCondition( "t.rmark = :rmark" );

    switch ( m_sort )
    {
        case FMARK: addSorting( "t.fmark" ); break;
        case RMARK: addSorting( "t.rmark" ); break;
        default: break;
    }

    return sqlite3_prepare16_v2( m_db, sql.utf16(), -1, &m_stmt, NULL );
}

// ----------------------------------------------------------------------------

int TranslationQuery::bind()
{
    int err = SQLITE_OK;
    if ( filterId ) err = bindInt64( ":id", filterId );
    if ( !err && filterSid ) err = bindInt64( ":sid", filterSid );
    if ( !err && filterFmark >= 0 ) err = bindInt( ":fmark", filterFmark );
    if ( !err && filterRmark >= 0 ) err = bindInt64( ":rmark", filterRmark );
    LogIfSqlError( err, "TranslationQuery::bind()" );
    return err;
}

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
