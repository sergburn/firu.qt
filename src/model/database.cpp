#include <QDebug>
#include <QDir>

#include "database.h"
#include "../firudebug.h"

Database* g_schema = NULL;

// ----------------------------------------------------------------------------

QString Database::getWordTableName( Lang lang )
{
    return QString( "entries_" ) + QLocale::languageToString( lang );
}

// ----------------------------------------------------------------------------

QString Database::getTransTableName( LangPair langs )
{
    return QString( "trans_" ) + 
        QLocale::languageToString( langs.first ) + "_" +
        QLocale::languageToString( langs.second );
}

// ----------------------------------------------------------------------------

Database::Database( QObject* parent )
:   QObject( parent ),
    m_db( NULL ),
    m_transactionLevel( 0 ),
    m_transactionError( 0 )
{
    int err = sqlite3_initialize();
    if ( err )
    {
        LogSqliteError( m_db, "Database" );
    }
}

// ----------------------------------------------------------------------------

Database::~Database()
{
    if ( m_db )
    {
        sqlite3_close( m_db );
    }
    sqlite3_shutdown();
    g_schema = NULL;
}

// ----------------------------------------------------------------------------

Database* Database::instance()
{
//    assert( g_schema );
    return g_schema;
}

// ----------------------------------------------------------------------------

Database* Database::open( const QString& dbPath, QObject* parent )
{
//    assert( !g_schema );
    g_schema = new Database( parent );
    if ( g_schema->doOpen( dbPath ) )
    {
        return g_schema;
    }
    else
    {
        delete g_schema;
    }
    return g_schema;
}

// ----------------------------------------------------------------------------

bool Database::doOpen( const QString& dbPath )
{
    QString path = QDir::toNativeSeparators( dbPath );
    qDebug() << "Db: " << path;

    int err = sqlite3_open( path.toUtf8().constData(), &m_db );
    if ( err == SQLITE_OK )
    {
        return true;
    }
    else if ( m_db )
    {
        sqlite3_close( m_db );
        m_db = NULL;
    }

    if ( err )
    {
        LogSqliteError( m_db, "open" );
        return false;
    }
    return true;
}

// ----------------------------------------------------------------------------

sqlite3* Database::db()
{
    return m_db;
}

// ----------------------------------------------------------------------------

int Database::sqlCallback( void* pSelf, int nCol, char** argv, char** colv )
{
    Database* self = reinterpret_cast<Database*>( pSelf );
    return self->onSqlCallback( nCol, argv, colv );
}

// ----------------------------------------------------------------------------

int Database::onSqlCallback( int, char**, char** )
{
    emit onSqlProgress();
    return 0;
}

// ----------------------------------------------------------------------------

bool Database::langTableExists( Lang lang )
{
    return tableExists( getWordTableName( lang ) );
}

// ----------------------------------------------------------------------------

bool Database::transTableExists( LangPair langs )
{
    return tableExists( getTransTableName( langs ) );
}

// ----------------------------------------------------------------------------

bool Database::tableExists( const QString& table )
{
    const char* KSqlFindTable = 
        "SELECT 1 FROM sqlite_master WHERE type='table' AND name='%1'";
    
    QString sql = QString( KSqlFindTable ).arg( table );

    char** azResult = NULL;
    int nRow = 0;
    int nCol = 0;
    char* errMsg = NULL;
    int err = sqlite3_get_table( m_db, sql.toUtf8().constData(), &azResult, &nRow, &nCol, &errMsg );
    if ( err == SQLITE_OK )
    {
        sqlite3_free_table( azResult );
        return ( nRow > 0 );
    }
    else
    {
        LogSqliteError( m_db, "tableExists" );
        return false;
    }
}

// ----------------------------------------------------------------------------

int Database::sqlGetTable( const QString& /*sql*/ )
{
    return -1;
}

// ----------------------------------------------------------------------------

int Database::sqlExecute( QString sql )
{
    int err = sqlite3_exec( m_db, sql.toUtf8().constData(), NULL, NULL, NULL );
    if ( err )
    {
        LogSqliteError( m_db, "sqlExecute" );
    }
    return err;
}

// ----------------------------------------------------------------------------

int Database::createLangTable( Lang lang )
{
    const char* KSqlCreateEntriesTable = 
        "CREATE TABLE IF NOT EXISTS %1 ( id INTEGER PRIMARY KEY AUTOINCREMENT, text TEXT );";
    QString langTableName = getWordTableName( lang );
    
    // Table
    QString sql = QString( KSqlCreateEntriesTable ).arg ( langTableName );
    int err = sqlExecute( sql );
    
    // Index on text
    if ( !err )
    {
        const char* KSqlCreateEntriesIndex = 
            "CREATE UNIQUE INDEX IF NOT EXISTS index_%1_text ON %1 ( text ASC );";
        
        QString sql = QString( KSqlCreateEntriesIndex ).arg ( langTableName );
        
        err = sqlExecute( sql );
    }
    
    if ( err != SQLITE_OK )
    {
        LogSqliteError( m_db, "createLangTable" );
        sqlExecute( QString( "DROP TABLE %1;" ).arg( langTableName ) );
    }
    
    return err;
}

// ----------------------------------------------------------------------------

int Database::createTransTable( LangPair langs )
{
    const char* KSqlCreateTransTable = 
        "CREATE TABLE IF NOT EXISTS %1 ( "
            "id INTEGER PRIMARY KEY AUTOINCREMENT, "
            "sid INTEGER NOT NULL REFERENCES %2 (id) ON DELETE CASCADE, "
            "text TEXT, "
            "fmark INTEGER DEFAULT 0, "
            "rmark INTEGER DEFAULT 0 );";
    
    QString transTableName = getTransTableName( langs );
    
    // Create the table
    QString sql = QString( KSqlCreateTransTable ).arg(
            transTableName, getWordTableName( langs.first ) );
    
    int err = sqlExecute( sql );
    if ( !err )
    {
        // Create indexes
        const char* KSqlCreateTransIndex = 
            "CREATE INDEX IF NOT EXISTS index_%1_%2 ON %1 (%2);";

        QString source = QLocale::languageToString( langs.first );
        QString target = QLocale::languageToString( langs.second );
        
        // 1. Foreign key
        // CREATE INDEX IF NOT EXISTS index_trans_fi_ru_sid ON trans_fi_ru (sid);
        sql = QString( KSqlCreateTransIndex ).arg( transTableName, "sid" );
        err = sqlExecute( sql );
        
        // 2. Index on text
        // CREATE INDEX index_trans_fi_ru_text ON trans_fi_ru ( text ASC );
        if ( !err )
        {
            sql = QString( KSqlCreateTransIndex ).arg ( transTableName, "text" );
            err = sqlExecute( sql );
        }

        // 3. Index on mark
        // CREATE INDEX IF NOT EXISTS index_trans_fi_ru_fmark ON trans_fi_ru (fmark ASC);
        if ( !err )
        {
            sql = QString( KSqlCreateTransIndex ).arg( transTableName, "fmark" );
            err = sqlExecute( sql );
        }
        // CREATE INDEX IF NOT EXISTS index_trans_fi_ru_rmark ON trans_fi_ru (fmark ASC);
        if ( !err )
        {
            sql = QString( KSqlCreateTransIndex ).arg( transTableName, "rmark" );
            err = sqlExecute( sql );
        }
    }
    
    if ( err != SQLITE_OK )
    {
        LogSqliteError( m_db, "createTransTable" );
        sqlExecute( QString( "DROP TABLE %1;" ).arg( transTableName ) );
    }
    
    return err;
}

// ----------------------------------------------------------------------------

bool Database::begin()
{
    if ( !inTransaction() )
    {
        m_transactionError = sqlExecute( QString("BEGIN TRANSACTION;") );
        if ( m_transactionError )
        {
            LogSqliteError( m_db, "begin" );
        }
    }

    if ( !m_transactionError )
    {
        m_transactionLevel++;
        return true;
    }

    return false;
}

// ----------------------------------------------------------------------------

bool Database::commit()
{
    if ( !inTransaction() )
    {
        return false;
    }
    else if ( m_transactionLevel > 1 )
    {
        m_transactionLevel--;
        return true;
    }
    else // m_transactionLevel == 1
    {
        if ( !m_transactionError )
        {
            m_transactionError = sqlExecute( QString("COMMIT TRANSACTION;") );
        }

        if ( m_transactionError )
        {
            LogSqliteError( m_db, "commit" );
            rollback();
        }
        else
        {
            emit onTransactionFinish( true );
        }

        m_transactionLevel = 0;
        return m_transactionError == 0;
    }
}

// ----------------------------------------------------------------------------

void Database::rollback()
{
    if ( m_transactionLevel > 1 )
    {
        m_transactionError = SQLITE_ABORT;
        m_transactionLevel--;
        return;
    }
    else
    {
        int err = sqlExecute( QString("ROLLBACK TRANSACTION;") );
        if ( err )
        {
            LogSqliteError( m_db, "rollback" );
        }
        emit onTransactionFinish( false );
    }
}

// ----------------------------------------------------------------------------

bool Database::inTransaction() const
{
    return m_transactionLevel > 0;
}

// ----------------------------------------------------------------------------

void Database::addQuery( Query::Ptr query, Lang src, Lang trg )
{
    QueryLangPairHash& allForClass = m_queries[ query->metaObject()->className() ];
    allForClass.insert( LangPair( src, trg ), query );
}

// ----------------------------------------------------------------------------

Query::Ptr Database::findQuery( const char* className, Lang src, Lang trg )
{
    if ( m_queries.contains( className ) )
    {
        QueryLangPairHash& allForClass = m_queries[className];
        LangPair langs( src, trg );
        if ( allForClass.contains( langs ) )
        {
            return allForClass[langs];
        }
    }
    return Query::Ptr();
}
