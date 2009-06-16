#include "query.h"
#include "database.h"
#include "sqlgenerator.h"
#include "../firudebug.h"

// ----------------------------------------------------------------------------

Query::Query( Database* db, Lang src, QObject* parent )
    :
    QObject( parent ),
    m_db( db->db() ),
    m_stmt( NULL ), m_sortAscending( true ),
    m_srcLang( src ), m_trgLang( QLocale::C )
{
}

// ----------------------------------------------------------------------------

Query::Query( Database* db, LangPair langs, QObject* parent )
    :
    QObject( parent ), 
    m_db( db->db() ),
    m_stmt( NULL ), m_sortAscending( true ),
    m_srcLang( langs.first ), m_trgLang( langs.second )
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
            LogSqliteError( m_db, "Query::start(), prepare" );
            return false;
        }
    }
    else
    {
        sqlite3_clear_bindings( m_stmt );
        sqlite3_reset( m_stmt );
    }

    int err = bind();
    if ( err )
    {
        LogSqliteError( m_db, "Query::start(), bind" );
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
            sqlite3_clear_bindings( m_stmt );
            sqlite3_reset( m_stmt );
            emit onQueryFinish( SUCCESS );
            return false;

        default: // error
            LogSqliteError( m_db, "Query::next()" );
            sqlite3_clear_bindings( m_stmt );
            sqlite3_reset( m_stmt );
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
        int steps = 0;
        while ( next() )
        {
            if ( steps % 10 == 0 )
            {
                emit onQueryProgress();
            }
        }
        err = sqlite3_errcode( m_db );
    }
    return SQLOK( err ) == SQLITE_OK;
}

// ----------------------------------------------------------------------------

int Query::error() const
{
    return sqlite3_errcode( m_db );
}

// ----------------------------------------------------------------------------

void Query::setPrimaryKey( qint64 id )
{
    m_pk = id;
}

// ----------------------------------------------------------------------------

void Query::reset()
{
    m_conditions = 0;
    m_sets = 0;
    m_sortAscending = true;
    sqlite3_clear_bindings( m_stmt );
    sqlite3_reset( m_stmt );
}

// ----------------------------------------------------------------------------

int Query::bindInt( const char* parameter, int value )
{
    int pos = sqlite3_bind_parameter_index( m_stmt, parameter );
    int err = sqlite3_bind_int( m_stmt, pos, value );
    if ( err ) LogSqliteError( m_db, "Query::bindInt()" );
    return err;
}

// ----------------------------------------------------------------------------

int Query::bindInt64( const char* parameter, qint64 value )
{
    int pos = sqlite3_bind_parameter_index( m_stmt, parameter );
    int err = sqlite3_bind_int64( m_stmt, pos, value );
    if ( err ) LogSqliteError( m_db, "Query::bindInt64()" );
    return err;
}

// ----------------------------------------------------------------------------

int Query::bindString( const char* parameter, const QString& value )
{
    int pos = sqlite3_bind_parameter_index( m_stmt, parameter );
    int err = sqlite3_bind_text16( m_stmt, pos, value.utf16(), (value.size() + 1) * 2, SQLITE_STATIC );
    if ( err ) LogSqliteError( m_db, "Query::bindString()" );
    return err;
}

// ----------------------------------------------------------------------------

int Query::bindPrimaryKey()
{
    int err = bindInt64( ":id", m_pk );
    if ( err ) LogSqliteError( m_db, "Query::bindPrimaryKey" );
    return err;
}

// ----------------------------------------------------------------------------

QString Query::selectBaseSql() const
{
    return SqlGenerator::selectBaseSql( m_tableName );
}

// ----------------------------------------------------------------------------

QString Query::countBaseSql() const
{
    return SqlGenerator::countBaseSql( m_tableName );
}

// ----------------------------------------------------------------------------

QString Query::updateBaseSql() const
{
    return SqlGenerator::updateBaseSql( m_tableName );
}

// ----------------------------------------------------------------------------

QString Query::deleteBaseSql() const
{
    return SqlGenerator::deleteBaseSql( m_tableName );
}
