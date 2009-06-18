#include "query.h"
#include "database.h"
#include "sqlgenerator.h"
#include "../firudebug.h"

// ----------------------------------------------------------------------------

Query::Query( Database* db )
    :
    QObject( db ),
    m_db( db->db() ),
    m_stmt( NULL )
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
        qDebug() << "Preparing SQL:" << sql;
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

    m_finalSql = sqlite3_sql( m_stmt );
    int err = bind();
    qDebug() << "Executing SQL:" << m_finalSql;
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
    sqlite3_clear_bindings( m_stmt );
    sqlite3_reset( m_stmt );
}

// ----------------------------------------------------------------------------

int Query::bindInt( const char* parameter, int value )
{
    int pos = sqlite3_bind_parameter_index( m_stmt, parameter );
    int err = sqlite3_bind_int( m_stmt, pos, value );
    if ( err ) LogSqliteError( m_db, "Query::bindInt()" );
    m_finalSql.replace( parameter, QString::number( value ) );
    return err;
}

// ----------------------------------------------------------------------------

int Query::bindInt64( const char* parameter, qint64 value )
{
    int pos = sqlite3_bind_parameter_index( m_stmt, parameter );
    int err = sqlite3_bind_int64( m_stmt, pos, value );
    if ( err ) LogSqliteError( m_db, "Query::bindInt64()" );
    m_finalSql.replace( parameter, QString::number( value ) );
    return err;
}

// ----------------------------------------------------------------------------

int Query::bindString( const char* parameter, const QString& value )
{
    int pos = sqlite3_bind_parameter_index( m_stmt, parameter );
    int err = sqlite3_bind_text16( m_stmt, pos, value.utf16(), (value.size() + 1) * 2, SQLITE_STATIC );
    if ( err ) LogSqliteError( m_db, "Query::bindString()" );
    m_finalSql.replace( parameter, QString( value ).append('\'').prepend('\'') );
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
