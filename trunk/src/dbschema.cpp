/*
 * dbschema.cpp
 *
 *  Created on: Apr 28, 2009
 *      Author: burnevsk
 */

#include "dbschema.h"
#include <QDebug>

DbSchema::DbSchema( QObject* parent )
   : QObject( parent ), 
   m_db( NULL ),
   m_insertToSource( NULL ),
   m_insertToTarget( NULL ),
   m_insertToTrans( NULL ),
   m_updateTrans( NULL ),
   m_selectTransBySid( NULL ),
   m_selectTransByTid( NULL ),
   m_selectTransByFmark( NULL ),
   m_selectTransByRmark( NULL ),
   m_lastSrc( QLocale::C ), m_lastTrg ( QLocale::C )
{
}

DbSchema::~DbSchema()
{
    freeStatments();
    if ( m_db )
    {
        sqlite3_close( m_db );
    }
}

bool DbSchema::open( const QString& dbPath )
{
    qDebug() << "Fi: " << QLocale::languageToString( QLocale::Finnish );
    qDebug() << "Ru: " << QLocale::languageToString( QLocale::Russian );
    qDebug() << "Db: " << dbPath;

    int err = sqlite3_open( dbPath.toUtf8().constData(), &m_db );
    if ( err == SQLITE_OK )
    {
        return true;
    }
    else if ( m_db )
    {
        QString msg( sqlite3_errmsg( m_db ) );
        qDebug() << msg;
        sqlite3_close( m_db );
        return false;
    }
    else
    {
        return false;
    }
}

QString getLangTableName( Lang lang )
{
    return QString( "entries_" ) + QLocale::languageToString( lang );
}

QString getTransTableName( Lang source, Lang target )
{
    return QString( "trans_" ) + 
        QLocale::languageToString( source ) + "_" +
        QLocale::languageToString( target );
}

int DbSchema::sqlCallback( void* pSelf, int nCol, char** argv, char** colv )
{
    DbSchema* self = reinterpret_cast<DbSchema*>( pSelf );
    return self->onSqlCallback( nCol, argv, colv );
}

int DbSchema::onSqlCallback( int, char**, char** )
{
    return 0;
}

bool DbSchema::langTableExists( Lang lang )
{
    return tableExists( getLangTableName( lang ) );
}

bool DbSchema::transTableExists( Lang src, Lang trg )
{
    return tableExists( getTransTableName( src, trg ) );
}

bool DbSchema::tableExists( const QString& table )
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
        return ( nRow > 1 );
    }
    else
    {
        return false;
    }
}

int DbSchema::sqlExecute( QString sql )
{
    char* errMsg = NULL;
    int err = sqlite3_exec( m_db, sql.toUtf8().constData(), NULL, NULL, &errMsg );
    qDebug() << "createLangTable: " << errMsg;
    if ( err == SQLITE_OK )
    {
        sqlite3_free( errMsg );
    }
    return err;
}

int DbSchema::createLangTable( Lang lang )
{
    const char* KSqlCreateEntriesTable = 
        "CREATE TABLE IF NOT EXISTS %1 ( id INTEGER PRIMARY KEY AUTOINCREMENT, text TEXT );";
    QString sql = QString( KSqlCreateEntriesTable ).arg ( getLangTableName( lang ) );
    int result = sqlExecute( sql );
    if ( result == SQLITE_OK )
    {
        const char* KSqlCreateEntriesIndex = 
            "CREATE INDEX IF NOT EXISTS index_%S_text ON %S ( text ASC );";
        QString sql = QString( KSqlCreateEntriesIndex ).arg (
                QLocale::languageToString( lang ), getLangTableName( lang ) );
        result = sqlExecute( sql );
    }
    
    return result;
}

int DbSchema::createTransTable( Lang src, Lang trg )
{
    const char* KSqlCreateTransTable = 
        "CREATE TABLE IF NOT EXISTS %1 ( "
            "sid INTEGER NOT NULL REFERENCES %2 (id) ON DELETE CASCADE, "
            "tid INTEGER NOT NULL REFERENCES %3 (id) ON DELETE CASCADE, "
            "fmark INTEGER DEFAULT 0, "
            "rmark INTEGER DEFAULT 0, "
            "PRIMARY KEY ( sid, tid ) );"; 
    
    // Create the table
    QString sql = QString( KSqlCreateTransTable ).arg(
            getTransTableName( src, trg ), getLangTableName( src ), getLangTableName( trg ) );
    
    int result = sqlExecute( sql );
    if ( result != SQLITE_OK ) return result;

    // Create indexes
    QString source = QLocale::languageToString( src );
    QString target = QLocale::languageToString( trg );
    
    // 1. Foreign keys
    const char* KSqlCreateTransIndex = 
        "CREATE INDEX IF NOT EXISTS index_%1_%2 ON %1 (%2);";
    
    // CREATE INDEX IF NOT EXISTS index_trans_fi_ru_sid ON trans_fi_ru (sid);
    // not needed, primary key can be used

    // CREATE INDEX IF NOT EXISTS index_trans_fi_ru_tid ON trans_fi_ru (tid);
    sql = QString( KSqlCreateTransIndex ).arg( getTransTableName( src, trg ), "tid" );
    result = sqlExecute( sql );
    if ( result != SQLITE_OK ) return result;
    
    // 2. Index on mark

    // CREATE INDEX IF NOT EXISTS index_trans_fi_ru_fmark ON trans_fi_ru (fmark ASC);
    sql = KSqlCreateTransIndex;
    sql = sql.arg( getTransTableName( src, trg ), "fmark" );
    result = sqlExecute( sql );
    if ( result != SQLITE_OK ) return result;

    // CREATE INDEX IF NOT EXISTS index_trans_fi_ru_rmark ON trans_fi_ru (fmark ASC);
    sql = KSqlCreateTransIndex;
    sql = sql.arg( getTransTableName( src, trg ), "rmark" );
    result = sqlExecute( sql );
    if ( result != SQLITE_OK ) return result;
    
    return SQLITE_OK;
}

int DbSchema::genStatements( Lang src, Lang trg )
{
    if ( m_lastSrc != src || m_lastTrg != trg )
    {
        freeStatments();
        QString sql;
        QByteArray sql8;

        const char* InsertToLang = "INSERT INTO %1 VALUES ( text = ?text );";
        sql = QString( InsertToLang ).arg( getLangTableName( src ) );
        int err = sqlite3_prepare_v2( m_db, sql.toUtf8().constData(), -1, &m_insertToSource, NULL );
        if ( err ) return err;
        
        sql = QString( InsertToLang ).arg( getLangTableName( trg ) );
        err = sqlite3_prepare_v2( m_db, sql.toUtf8().constData(), -1, &m_insertToTarget, NULL );
        if ( err ) return err;

        m_lastSrc = src;
        m_lastTrg = trg;
    }

    return 0;
}

void DbSchema::freeStatments()
{
    sqlite3_free( m_insertToSource );
    sqlite3_free( m_insertToTarget );
    sqlite3_free( m_insertToTrans );
    sqlite3_free( m_selectTransBySid );
    sqlite3_free( m_selectTransByTid );
    sqlite3_free( m_selectTransByFmark );
    sqlite3_free( m_selectTransByRmark );
}
