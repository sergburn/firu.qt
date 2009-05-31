#include "query.h"

// ----------------------------------------------------------------------------

Query::Query( sqlite3* db, Lang src, Lang trg )
    :
    m_db( db ),
    m_stmt( NULL ), m_sortAscending( true ),
    m_srcLang( src ), m_trgLang( trg )
{
}

// ----------------------------------------------------------------------------

Query::~Query()
{
    sqlite3_finalize( m_stmt );
}

// ----------------------------------------------------------------------------

int Query::start()
{
    int err = SQLITE_OK;
    if ( !m_stmt )
    {
        err = prepare();
    }

    if ( !err )
    {
        sqlite3_clear_bindings( stmt );
        sqlite3_reset( stmt );

        err = bind();
    }
    return err;
}

// ----------------------------------------------------------------------------

bool Query::next()
{
    int err = sqlite3_step( m_stmt );
    switch ( err )
    {
        case SQLITE_ROW:
            read();
            return true;
        default: // error
            LogSqliteError( "Query::next()" );
            // fall through
        case SQLITE_DONE:
            sqlite3_clear_bindings( stmt );
            sqlite3_reset( stmt );
            return false;
    }
}

// ----------------------------------------------------------------------------

int Query::addCondition( QString& sql, const char* condition )
{
    if ( m_conditions == 0 ) sql.append( "WHERE " );
    if ( m_conditions > 0 ) sql.append( " AND " );

    sql.append( condition );
    m_conditions++;
}

// ----------------------------------------------------------------------------

int Query::bindInt( const char* parameter, int value )
{
    int pos = sqlite3_bind_parameter_index( m_stmt, parameter );
    int err = sqlite3_bind_int( stmt, pos, value );
    if ( err ) LogSqliteError( "Query::bindInt()" );
    return err;
}

// ----------------------------------------------------------------------------

int Query::bindInt64( const char* parameter, qint64 value )
{
    int pos = sqlite3_bind_parameter_index( m_stmt, parameter );
    int err = sqlite3_bind_int64( stmt, pos, value );
    if ( err ) LogSqliteError( "Query::bindInt64()" );
    return err;
}

// ----------------------------------------------------------------------------

int Query::bindString( const char* parameter, const QString& value )
{
    int pos = sqlite3_bind_parameter_index( stmt, parameter );
    int err = sqlite3_bind_text16( stmt, pos, value.utf16(), (value.size() + 1) * 2, SQLITE_STATIC );
    if ( err ) LogSqliteError( "Query::bindString()" );
    return err;
}

// ----------------------------------------------------------------------------

int Query::addSorting( QString& sql, const char* field )
{
    QString sort( " ORDER BY %1" );
    sort.arg( field );
    if ( !m_sortAscending ) sort.append( " DESC" );
    sql.append( sort );
}
