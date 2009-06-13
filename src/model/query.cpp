#include "query.h"

// ----------------------------------------------------------------------------

Query::Query( sqlite3* db, Lang src, QObject* parent )
    :
    QObject( parent ),
    m_db( db ),
    m_stmt( NULL ), m_sortAscending( true ),
    m_srcLang( src ), m_trgLang( QLocale::C )
{
}

// ----------------------------------------------------------------------------

Query::Query( sqlite3* db, Lang src, Lang trg, QObject* parent )
    :
    QObject( parent ), 
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

bool Query::start()
{
    if ( !m_stmt )
    {
        QString sql = buildSql();
        int err = sqlite3_prepare16_v2( m_db, sql.utf16(), -1, &m_stmt, NULL );
        if ( err )
        {
            LogSqliteError( "Query::start(), prepare" );
            return false;
        }
    }
    else
    {
        sqlite3_clear_bindings( stmt );
        sqlite3_reset( stmt );
    }

    int err = bind();
    if ( err )
    {
        LogSqliteError( "Query::start(), bind" );
        return false;
    }
    return true;
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

        case SQLITE_DONE:
            sqlite3_clear_bindings( stmt );
            sqlite3_reset( stmt );
            emit onQueryFinish( SUCCESS );
            return false;

        default: // error
            LogSqliteError( "Query::next()" );
            sqlite3_clear_bindings( stmt );
            sqlite3_reset( stmt );
            emit onQueryFinish( FAILURE );
            return false;
    }
}

// ----------------------------------------------------------------------------

bool Query::execute()
{
    int err = start();
    if ( !err )
    {
        while ( next() );
        err = sqlite3_error( m_db );
    }
    return SQLOK( err ) == SQLITE_OK;
}

// ----------------------------------------------------------------------------

int Query::error() const
{
    return sqlite3_error( m_db );
}

// ----------------------------------------------------------------------------

int Query::queryProgress()
{
    emit onQueryProgress();
}

// ----------------------------------------------------------------------------

void Query::setPrimaryKey( qint64 id )
{
    m_pk = id;
}

// ----------------------------------------------------------------------------

int Query::reset()
{
    m_conditions = 0;
    m_sets = 0;
    m_sortAscending = true;
    sqlite3_clear_bindings( stmt );
    sqlite3_reset( stmt );
    doReset();
}

// ----------------------------------------------------------------------------

void Query::addCondition( QString& sql, const char* condition )
{
    if ( m_conditions == 0 ) sql.append( "WHERE " );
    if ( m_conditions > 0 ) sql.append( " AND " );

    sql.append( condition );
    m_conditions++;
}

// ----------------------------------------------------------------------------

void Query::addPrimaryKeyCondition( QString& sql)
{
    addCondition( "id = :id" );
}

// ----------------------------------------------------------------------------

void Query::addSet( QString& sql, const char* expr )
{
    if ( m_sets > 0 ) sql.append( ", " );
    sql.append( expr );
    m_sets++;
}

// ----------------------------------------------------------------------------

void Query::addSorting( QString& sql, const char* field )
{
    QString sort( " ORDER BY %1" );
    sort.arg( field );
    if ( !m_sortAscending ) sort.append( " DESC" );
    sql.append( sort );
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

int Query::bindPrimaryKey()
{
    int err = bindInt64( ":id", m_pk );
    LogIfSqlError( err, "Query::bindPrimaryKey" );
    return err;
}

// ----------------------------------------------------------------------------

QString Query::createPattern( const QString& text, TextMatch match )
{
    switch ( match )
    {
        case StartsWith:
            return pattern.append('%');
        case Contains:
            return pattern.prepend('%').append('%');
        case EndsWith:
            return pattern.prepend('%');
        case FullMatch:
        default:
            return text;
    }
}

// ----------------------------------------------------------------------------

QString Query::selectBaseSql() const
{
    return QString( "SELECT * FROM %1" ).arg( m_tableName );
}

// ----------------------------------------------------------------------------

QString Query::updateBaseSql() const
{
    return QString( "UPDATE %1 SET" ).arg( m_tableName );
}

// ----------------------------------------------------------------------------

QString Query::deleteBaseSql() const
{
    return QString( "DELETE FROM %1" ).arg( m_tableName );
}
