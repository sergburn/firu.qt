#include "query.h"
#include "database.h"
#include "sqlgenerator.h"
#include "../firudebug.h"
#ifndef FIRU_USE_SQLITE
#include <QSqlError>
#endif
#include <QVariant>

// ----------------------------------------------------------------------------

Query::Query( Database* db ) :
    QObject( db ),
    m_db( db->db() )
#ifdef FIRU_USE_SQLITE
    , m_stmt( NULL )
#endif      
{
#ifndef FIRU_USE_SQLITE
    m_query.setForwardOnly( true );
#endif
}

// ----------------------------------------------------------------------------

Query::~Query()
{
    //    sqlite3_finalize( m_stmt );
}

// ----------------------------------------------------------------------------

bool Query::start()
{
#ifdef FIRU_USE_SQLITE
    if ( !m_stmt )
    {
        QString sql = buildSql();
        qDebug() << "Preparing SQL:" << sql;
        int err = sqlite3_prepare16_v2( m_db, sql.utf16(), -1, &m_stmt, NULL );
        if ( err )
        {
            LogSqlError( m_db, "Query::start(), prepare" );
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
        LogSqlError( m_db, "Query::start(), bind" );
        return false;
    }
#else
    if ( !m_query.isActive() )
    {
        QString sql = buildSql();
        qDebug() << "Preparing SQL:" << sql;
        if ( !m_query.prepare( sql ) )
        {
            LogSqlError( m_db, "Query::start(), prepare" );
            return false;
        }
    }

    m_finalSql = m_query.lastQuery();
    if ( bind() != 0 )
    {
        LogSqlError( m_db, "Query::start(), bind" );
        return false;
    }
    qDebug() << "Executing SQL:" << m_finalSql;
    if ( !m_query.exec() )
    {
        LogSqlError( m_db, "Query::start(), exec" );
        return false;
    }
    qDebug() << "Query exec done";
#endif
    
    return true;
}

// ----------------------------------------------------------------------------

bool Query::next()
{
#ifdef FIRU_USE_SQLITE
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
            LogSqlError( m_db, "Query::next()" );
            sqlite3_clear_bindings( m_stmt );
            sqlite3_reset( m_stmt );
            emit onQueryFinish( FAILURE );
            return false;
    }
#else
    if ( m_query.next() )
    {
        read();
        return true;
    }
    else if ( m_db.lastError().type() != QSqlError::NoError )
    {
        m_query.finish();
        LogSqlError( m_db, "Query::next()" );
        emit onQueryFinish( FAILURE );
        return false;
    }
    else
    {
        m_query.finish();
        emit onQueryFinish( SUCCESS );
        return false;
    }
#endif
}

// ----------------------------------------------------------------------------

bool Query::execute()
{
#ifdef FIRU_USE_SQLITE
    if ( start() )
    {
        int steps = 0;
        while ( next() )
        {
            if ( steps % 10 == 0 )
            {
                emit onQueryProgress();
            }
        }
        int err = sqlite3_errcode( m_db );
        return SQLOK( err ) == SQLITE_OK;
    }
#else
    return start();
#endif
    return false;
}

// ----------------------------------------------------------------------------

void Query::setPrimaryKey( qint64 id )
{
    m_pk = id;
}

// ----------------------------------------------------------------------------

void Query::reset()
{
#ifdef FIRU_USE_SQLITE
    sqlite3_clear_bindings( m_stmt );
    sqlite3_reset( m_stmt );
#else
    m_query.clear();
#endif
}

// ----------------------------------------------------------------------------

int Query::bindInt( const char* parameter, int value )
{
#ifdef FIRU_USE_SQLITE
    int pos = sqlite3_bind_parameter_index( m_stmt, parameter );
    int err = sqlite3_bind_int( m_stmt, pos, value );
    if ( err )
        LogSqlError( m_db, "Query::bindInt()" );
    m_finalSql.replace( parameter, QString::number( value ) );
    return err;
#else
    m_query.bindValue( QString( parameter ), QVariant( value ) );
    m_finalSql.replace( parameter, QString::number( value ) );
    return 0;
#endif    
}

// ----------------------------------------------------------------------------

int Query::bindUint( const char* parameter, uint value )
{
#ifdef FIRU_USE_SQLITE
    int pos = sqlite3_bind_parameter_index( m_stmt, parameter );
    int err = sqlite3_bind_int( m_stmt, pos, value );
    if ( err )
        LogSqlError( m_db, "Query::bindUint()" );
    m_finalSql.replace( parameter, QString::number( value ) );
    return err;
#else
    m_query.bindValue( QString( parameter ), QVariant( value ) );
    m_finalSql.replace( parameter, QString::number( value ) );
    return 0;
#endif
}

// ----------------------------------------------------------------------------

int Query::bindInt64( const char* parameter, qint64 value )
{
#ifdef FIRU_USE_SQLITE
    int pos = sqlite3_bind_parameter_index( m_stmt, parameter );
    int err = sqlite3_bind_int64( m_stmt, pos, value );
    if ( err )
        LogSqlError( m_db, "Query::bindInt64()" );
    m_finalSql.replace( parameter, QString::number( value ) );
    return err;
#else
    m_query.bindValue( QString( parameter ), QVariant( value ) );
    m_finalSql.replace( parameter, QString::number( value ) );
    return 0;
#endif    
}

// ----------------------------------------------------------------------------

int Query::bindUint64( const char* parameter, quint64 value )
{
#ifdef FIRU_USE_SQLITE
    int pos = sqlite3_bind_parameter_index( m_stmt, parameter );
    int err = sqlite3_bind_int64( m_stmt, pos, value );
    if ( err )
        LogSqlError( m_db, "Query::bindUint64()" );
    m_finalSql.replace( parameter, QString::number( value ) );
    return err;
#else
    m_query.bindValue( QString( parameter ), QVariant( value ) );
    m_finalSql.replace( parameter, QString::number( value ) );
    return 0;
#endif
}

// ----------------------------------------------------------------------------

int Query::bindString( const char* parameter, const QString& value )
{
#ifdef FIRU_USE_SQLITE
    int pos = sqlite3_bind_parameter_index( m_stmt, parameter );
    int err = sqlite3_bind_text16( m_stmt, pos, value.utf16(), ( value.size() + 1 ) * 2,
        SQLITE_STATIC );
    if ( err )
        LogSqlError( m_db, "Query::bindString()" );
    m_finalSql.replace( parameter, QString( value ).append( '\'' ).prepend( '\'' ) );
    return err;
#else
    m_query.bindValue( QString( parameter ), QVariant( value ) );
    m_finalSql.replace( parameter, value );
    return 0;
#endif    
}

// ----------------------------------------------------------------------------

int Query::bindPrimaryKey()
{
    int err = bindInt64( ":id", m_pk );
    if ( err )
    {
        LogSqlError( m_db, "Query::bindPrimaryKey" );
    }
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
