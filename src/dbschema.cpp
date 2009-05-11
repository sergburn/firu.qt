/*
 * dbschema.cpp
 *
 *  Created on: Apr 28, 2009
 *      Author: burnevsk
 */

#include "dbschema.h"
#include <QDebug>
#include <QDir>

#define SQLOK( _e ) ( ((_e) == SQLITE_OK || (_e) == SQLITE_DONE || (_e) == SQLITE_ROW ) ? SQLITE_OK : (_e) )

// ----------------------------------------------------------------------------

QString getEntryTableName( Lang lang )
{
    return QString( "entries_" ) + QLocale::languageToString( lang );
}

// ----------------------------------------------------------------------------
QString getTransTableName( Lang source, Lang target )

{
    return QString( "trans_" ) + 
        QLocale::languageToString( source ) + "_" +
        QLocale::languageToString( target );
}

// ----------------------------------------------------------------------------

DbSchema::DbSchema( QObject* parent )
   : QObject( parent ), 
   m_db( NULL ),
   m_insertEntry( NULL ),
   m_insertTrans( NULL ),
   m_updateTrans( NULL ),
   m_selectEntryById( NULL ),
   m_selectEntriesByPattern( NULL ),
   m_selectTransByPattern( NULL ),
   m_selectTransById( NULL ),
   m_selectTransBySid( NULL ),
   m_selectTransByText( NULL ),
   m_selectTransByFmark( NULL ),
   m_selectTransByRmark( NULL ),
   m_lastSrc( QLocale::C ), m_lastTrg ( QLocale::C )
{
    int err = sqlite3_initialize();
    if ( err )
    {
        LogSqliteError( "DbSchema" );
    }
}

// ----------------------------------------------------------------------------

DbSchema::~DbSchema()
{
    freeStatements();
    if ( m_db )
    {
        sqlite3_close( m_db );
    }
    sqlite3_shutdown();
}

// ----------------------------------------------------------------------------

bool DbSchema::open( const QString& dbPath )
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
    }

    if ( err )
    {
        LogSqliteError( "open" );
        return false;
    }
    return true;
}

// ----------------------------------------------------------------------------

int DbSchema::addEntry( Lang lang, const QString& text, qint64& id )
{
    int err = SQLITE_OK;
    prepareStatements( lang, m_lastTrg );
    
    // bind
    int pos = sqlite3_bind_parameter_index( m_insertEntry, ":text" );
    err = sqlite3_bind_text16( m_insertEntry, pos, text.utf16(), (text.size() + 1) * 2, SQLITE_STATIC );

    // execute
    if ( !err ) 
    {
        err = sqlite3_step( m_insertEntry );
    }
    
    sqlite3_clear_bindings( m_insertEntry );
    sqlite3_reset( m_insertEntry );

    if ( err == SQLITE_DONE )
    {
        id = sqlite3_last_insert_rowid( m_db );
        err = SQLITE_OK;
    }
    else if ( err != SQLITE_OK )
    {
        LogSqliteError( "addEntry" );
        id = 0;
    }
    return err;
}

// ----------------------------------------------------------------------------

int DbSchema::addTranslation( Lang src, Lang trg, qint64 sid, const QString& text, qint64& id )
{
    int err = SQLITE_OK;
    prepareStatements( src, trg );
    
    // Sid param
    int pos = sqlite3_bind_parameter_index( m_insertTrans, ":sid" );
    err = sqlite3_bind_int64( m_insertTrans, pos, sid );

    // Text param
    if ( !err )
    {
        pos = sqlite3_bind_parameter_index( m_insertTrans, ":text" );
        err = sqlite3_bind_text16( m_insertTrans, pos, text.utf16(), (text.size() + 1) * 2, SQLITE_STATIC );
    }

    // execute
    if ( !err ) 
    {
        err = sqlite3_step( m_insertTrans );
    }
    
    sqlite3_clear_bindings( m_insertTrans );
    sqlite3_reset( m_insertTrans );

    if ( err == SQLITE_DONE )
    {
        id = sqlite3_last_insert_rowid( m_db );
        err = SQLITE_OK;
    }
    else if ( err != SQLITE_OK )
    {
        LogSqliteError( "addTranslation" );
        id = 0;
    }
    return err;
}

// ----------------------------------------------------------------------------

int DbSchema::getEntry( Lang lang, const QString& text, EntryRecord& record )
{
    prepareStatements( lang, m_lastTrg );
    int pos = sqlite3_bind_parameter_index( m_selectEntriesByPattern, ":text" );
    int err = sqlite3_bind_text16( m_selectEntriesByPattern, pos, text.utf16(), (text.size() + 1) * 2, SQLITE_STATIC );
    if ( !err ) err = nextEntryRecord( m_selectEntriesByPattern, record );
    sqlite3_clear_bindings( m_selectEntriesByPattern );
    sqlite3_reset( m_selectEntriesByPattern );
    return SQLOK( err );
}
    
// ----------------------------------------------------------------------------

int DbSchema::getEntry( Lang lang, qint64 id, EntryRecord& record )
{
    prepareStatements( lang, m_lastTrg );
    int pos = sqlite3_bind_parameter_index( m_selectEntryById, ":id" );
    int err = sqlite3_bind_int64( m_selectEntryById, pos, id );
    if ( !err ) err = nextEntryRecord( m_selectEntryById, record );
    sqlite3_clear_bindings( m_selectEntryById );
    sqlite3_reset( m_selectEntryById );
    return SQLOK( err );
}

// ----------------------------------------------------------------------------

QList<DbSchema::EntryRecord> DbSchema::getEntries( Lang lang, const QString& pattern )
{
    QList<DbSchema::EntryRecord> list;
    QString pat = pattern + "%";

    int err = prepareStatements( lang, m_lastTrg );
    if ( !err )
    {
        sqlite3_stmt* stmt = m_selectEntriesByPattern;
        sqlite3_clear_bindings( stmt );
        int pos = sqlite3_bind_parameter_index( stmt, ":pattern" );
        err = sqlite3_bind_text16( stmt, pos, pat.utf16(), (pat.size() + 1) * 2, SQLITE_STATIC );
        if ( !err ) list = getAllEntryRecords( stmt );
    }
    return list;
}

// ----------------------------------------------------------------------------

QList<DbSchema::EntryRecord> DbSchema::getAllEntryRecords( sqlite3_stmt* stmt )
{
    QList<DbSchema::EntryRecord> list;
    forever
    {
        EntryRecord record;
        if ( nextEntryRecord( stmt, record ) == SQLITE_ROW )
            list.append( record );
        else
            break;
    }

    sqlite3_clear_bindings( stmt );
    sqlite3_reset( stmt );
    return list;
}

// ----------------------------------------------------------------------------

int DbSchema::nextEntryRecord( sqlite3_stmt* stmt, EntryRecord& record )
{
    int err = sqlite3_step( stmt );
    switch ( err ) 
    {
        case SQLITE_ROW:
            readEntryRecord( stmt, record );
            break;
        case SQLITE_DONE:
            err = SQLITE_OK;
            break;
        default: // error
            LogSqliteError( "nextEntryRecord" );
            break;
    }
    return err;
}

// ----------------------------------------------------------------------------

void DbSchema::readEntryRecord( sqlite3_stmt* stmt, EntryRecord& record )
{
    record.id = sqlite3_column_int64( stmt, 0 );
    record.text = QString::fromUtf8( (const char*) sqlite3_column_text( stmt, 1 ) );
}

// ----------------------------------------------------------------------------

QList<DbSchema::TransViewRecord> DbSchema::getTranslationsByEntry( Lang src, Lang trg, qint64 sid )
{
    prepareStatements( src, trg );
    sqlite3_stmt* stmt = m_selectTransBySid;
    int pos = sqlite3_bind_parameter_index( stmt, ":sid" );
    int err = sqlite3_bind_int64( stmt, pos, sid );
    if ( !err )
        return getAllTransViewRecords( stmt );
    else
        return QList<DbSchema::TransViewRecord>();
}

// ----------------------------------------------------------------------------

QList<DbSchema::TransViewRecord> DbSchema::getTranslationsByPattern( Lang src, Lang trg, const QString& pattern )
{
    QString pat = pattern + "%";
    prepareStatements( src, trg );
    sqlite3_stmt* stmt = m_selectTransByPattern;
    int pos = sqlite3_bind_parameter_index( stmt, ":pattern" );
    int err = sqlite3_bind_text16( stmt, pos, pat.utf16(), (pat.size() + 1) * 2, SQLITE_STATIC );
    if ( !err )
        return getAllTransViewRecords( stmt );
    else
        return QList<TransViewRecord>();
}

// ----------------------------------------------------------------------------

QList<DbSchema::TransViewRecord> DbSchema::getAllTransViewRecords( sqlite3_stmt* stmt )
{
    QList<DbSchema::TransViewRecord> list;
    forever
    {
        TransViewRecord record;
        if ( nextTransViewRecord( stmt, record ) == SQLITE_ROW )
            list.append( record );
        else
            break;
    }

    sqlite3_clear_bindings( stmt );
    sqlite3_reset( stmt );
    return list;
}

// ----------------------------------------------------------------------------

int DbSchema::nextTransViewRecord( sqlite3_stmt* stmt, TransViewRecord& record )
{
    int err = sqlite3_step( stmt );
    switch ( err ) 
    {
        case SQLITE_ROW:
            readTransViewRecord( stmt, record );
            break;
        case SQLITE_DONE:
            err = SQLITE_OK;
            break;
        default: // error
            LogSqliteError( "nextTransViewRecord" );
            break;
    }
    return err;
}

// ----------------------------------------------------------------------------

void DbSchema::readTransViewRecord( sqlite3_stmt* stmt, TransViewRecord& record )
{
    // SELECT t.id, t.sid, /*e.text,*/ t.text, t.fmark, t.rmark
    record.id = sqlite3_column_int64( stmt, 0 );
    record.sid = sqlite3_column_int64( stmt, 1 );
    record.target = QString::fromUtf8( (const char*) sqlite3_column_text( stmt, 2 ) );
    record.fmark = sqlite3_column_int( stmt, 3 );
    record.rmark = sqlite3_column_int( stmt, 4 );
    //record.source = QString::fromUtf8( (const char*) sqlite3_column_text( stmt, 5 ) );
}

// ----------------------------------------------------------------------------

int DbSchema::saveTranslationMarks( Lang src, Lang trg, TransViewRecord r )
{
    prepareStatements( src, trg );
    sqlite3_stmt* stmt = m_updateTrans;

    int pos = sqlite3_bind_parameter_index( stmt, ":id" );
    int err = sqlite3_bind_int( stmt, pos, r.id );
    if ( !err )
    {
        pos = sqlite3_bind_parameter_index( stmt, ":fmark" );
        err = sqlite3_bind_int( stmt, pos, r.fmark );
    }
    if ( !err )
    {
        pos = sqlite3_bind_parameter_index( stmt, ":rmark" );
        err = sqlite3_bind_int( stmt, pos, r.rmark );
    }
    if ( !err )
    {
        err = sqlite3_step( stmt );
    }
    if ( err != SQLITE_DONE )
    {
        LogSqliteError( "saveTranslationMarks" );
    }
    sqlite3_clear_bindings( stmt );
    sqlite3_reset( stmt );
    return SQLOK( err );
}

// ----------------------------------------------------------------------------

int DbSchema::sqlCallback( void* pSelf, int nCol, char** argv, char** colv )
{
    DbSchema* self = reinterpret_cast<DbSchema*>( pSelf );
    return self->onSqlCallback( nCol, argv, colv );
}

// ----------------------------------------------------------------------------

int DbSchema::onSqlCallback( int, char**, char** )
{
    return 0;
}

// ----------------------------------------------------------------------------

int DbSchema::sqlProgress( void* pSelf )
{
    DbSchema* self = reinterpret_cast<DbSchema*>( pSelf );
    return self->onSqlProgress();
}

// ----------------------------------------------------------------------------

int DbSchema::onSqlProgress()
{
    emit onLongOpProgress();
    return 0;
}

// ----------------------------------------------------------------------------

bool DbSchema::langTableExists( Lang lang )
{
    return tableExists( getEntryTableName( lang ) );
}

// ----------------------------------------------------------------------------

bool DbSchema::transTableExists( Lang src, Lang trg )
{
    return tableExists( getTransTableName( src, trg ) );
}

// ----------------------------------------------------------------------------

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
        return ( nRow > 0 );
    }
    else
    {
        LogSqliteError( "tableExists" );
        return false;
    }
}

// ----------------------------------------------------------------------------

int DbSchema::sqlGetTable( const QString& /*sql*/ )
{
    return -1;
}

// ----------------------------------------------------------------------------

int DbSchema::sqlExecute( QString sql )
{
//    sqlite3_progress_handler( m_db, 10, sqlCallback, this );
    int err = sqlite3_exec( m_db, sql.toUtf8().constData(), NULL, NULL, NULL );
    if ( err )
    {
        LogSqliteError( "sqlExecute" );
    }
    return err;
}

// ----------------------------------------------------------------------------

int DbSchema::createLangTable( Lang lang )
{
    const char* KSqlCreateEntriesTable = 
        "CREATE TABLE IF NOT EXISTS %1 ( id INTEGER PRIMARY KEY AUTOINCREMENT, text TEXT );";
    QString langTableName = getEntryTableName( lang );
    
    // Table
    QString sql = QString( KSqlCreateEntriesTable ).arg ( langTableName );
    int err = sqlExecute( sql );
    
    // Index on text
    if ( !err )
    {
        const char* KSqlCreateEntriesIndex = 
            "CREATE INDEX IF NOT EXISTS index_%1_text ON %1 ( text ASC );";
        
        QString sql = QString( KSqlCreateEntriesIndex ).arg ( langTableName );
        
        err = sqlExecute( sql );
    }
    
    if ( err != SQLITE_OK )
    {
        LogSqliteError( "createLangTable" );
        sqlExecute( QString( "DROP TABLE %1;" ).arg( langTableName ) );
    }
    
    return err;
}

// ----------------------------------------------------------------------------

int DbSchema::createTransTable( Lang src, Lang trg )
{
    const char* KSqlCreateTransTable = 
        "CREATE TABLE IF NOT EXISTS %1 ( "
            "id INTEGER PRIMARY KEY AUTOINCREMENT, "
            "sid INTEGER NOT NULL REFERENCES %2 (id) ON DELETE CASCADE, "
            "text TEXT, "
            "fmark INTEGER DEFAULT 0, "
            "rmark INTEGER DEFAULT 0 );";
    
    QString transTableName = getTransTableName( src, trg );
    
    // Create the table
    QString sql = QString( KSqlCreateTransTable ).arg(
            transTableName, getEntryTableName( src ) );
    
    int err = sqlExecute( sql );
    if ( !err )
    {
        // Create indexes
        const char* KSqlCreateTransIndex = 
            "CREATE INDEX IF NOT EXISTS index_%1_%2 ON %1 (%2);";

        QString source = QLocale::languageToString( src );
        QString target = QLocale::languageToString( trg );
        
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
        LogSqliteError( "createTransTable" );
        sqlExecute( QString( "DROP TABLE %1;" ).arg( transTableName ) );
    }
    
    return err;
}

// ----------------------------------------------------------------------------

bool DbSchema::begin()
{
    int err = sqlExecute( QString("BEGIN TRANSACTION;") );
    if ( err )
    {
        LogSqliteError( "begin" );
    }
    return !err;
}

// ----------------------------------------------------------------------------

bool DbSchema::commit()
{
    int err = sqlExecute( QString("COMMIT TRANSACTION;") );
    if ( err )
    {
        LogSqliteError( "begin" );
        rollback();
    }
    return !err;
}

// ----------------------------------------------------------------------------

void DbSchema::rollback()
{
    int err = sqlExecute( QString("ROLLBACK TRANSACTION;") );
    if ( err )
    {
        LogSqliteError( "begin" );
    }
}

// ----------------------------------------------------------------------------

int DbSchema::prepareStatements( Lang src, Lang trg )
{
    if ( m_lastSrc != src || m_lastTrg != trg )
    {
        freeStatements();
        QString sql;
        QString entryTableName = getEntryTableName( src );
        QString transTableName = getTransTableName( src, trg );

        // Insert to Entries
        const char* InsertText = "INSERT INTO %1 ( text ) VALUES ( :text );";

        sql = QString( InsertText ).arg( entryTableName );
        int err = sqlite3_prepare16_v2( m_db, sql.utf16(), -1, &m_insertEntry, NULL );

        // Insert to Trans
        const char* InsertToTrans = "INSERT INTO %1 ( sid, text ) VALUES ( :sid, :text );";
        if ( !err )
        {
            sql = QString( InsertToTrans ).arg( transTableName );
            err = sqlite3_prepare16_v2( m_db, sql.utf16(), -1, &m_insertTrans, NULL );
        }

        // Selects by Id
        const char* SelectById = "SELECT * FROM %1 WHERE id = :id;";
        if ( !err )
        {
            sql = QString( SelectById ).arg( entryTableName );
            err = sqlite3_prepare16_v2( m_db, sql.utf16(), -1, &m_selectEntryById, NULL );
        }
        if ( !err )
        {
            sql = QString( SelectById ).arg( transTableName );
            err = sqlite3_prepare16_v2( m_db, sql.utf16(), -1, &m_selectTransById, NULL );
        }

        // TransView
        const char* SelectTransView =
            "SELECT t.id, t.sid, t.text, t.fmark, t.rmark "
            "FROM %1 as t "
            "WHERE t.sid = :sid;";
        if ( !err )
        {
            sql = QString( SelectTransView ).arg( transTableName );
            err = sqlite3_prepare16_v2( m_db, sql.utf16(), -1, &m_selectTransBySid, NULL );
        }

        // Select by Pattern
        const char* SelectByPattern = "SELECT * FROM %1 WHERE text LIKE :pattern;";
        if ( !err )
        {
            sql = QString( SelectByPattern ).arg( entryTableName );
            err = sqlite3_prepare16_v2( m_db, sql.utf16(), -1, &m_selectEntriesByPattern, NULL );
        }
        if ( !err )
        {
            sql = QString( SelectByPattern ).arg( transTableName );
            err = sqlite3_prepare16_v2( m_db, sql.utf16(), -1, &m_selectTransByPattern, NULL );
        }

        // Update Trans
        const char* UpdateTrans = "UPDATE %1 SET fmark = :fmark, rmark= :rmark WHERE id = :id;";
        if ( !err )
        {
            sql = QString( UpdateTrans ).arg( transTableName );
            err = sqlite3_prepare16_v2( m_db, sql.utf16(), -1, &m_updateTrans, NULL );
        }

        if ( !err )
        {
            m_lastSrc = src;
            m_lastTrg = trg;
        }
        else
        {
            LogSqliteError( "genStatements" );
            freeStatements();
            return err;
        }
    }

    return 0;
}

// ----------------------------------------------------------------------------

void DbSchema::freeStatements()
{
    m_lastSrc = m_lastTrg = QLocale::C;
    sqlite3_finalize( m_insertEntry );              m_insertEntry = NULL;
    sqlite3_finalize( m_insertTrans );              m_insertTrans  = NULL;
    sqlite3_finalize( m_selectEntriesByPattern );   m_selectEntriesByPattern = NULL;
    sqlite3_finalize( m_selectTransByPattern );     m_selectTransByPattern = NULL;
    sqlite3_finalize( m_selectTransById );          m_selectTransById = NULL;
    sqlite3_finalize( m_selectTransBySid );         m_selectTransBySid = NULL;
    sqlite3_finalize( m_selectTransByText );        m_selectTransByText = NULL;
    sqlite3_finalize( m_selectTransByFmark );       m_selectTransByFmark = NULL;
    sqlite3_finalize( m_selectTransByRmark );       m_selectTransByRmark = NULL;
}

// ----------------------------------------------------------------------------

void DbSchema::LogSqliteError( const char* where )
{
    QString err( sqlite3_errmsg( m_db ) );
    qDebug() << 
        "SqLite err in " << where << 
        ": err " << sqlite3_errcode( m_db ) << 
        ", exterr "  << sqlite3_extended_errcode( m_db ) <<
        ", msg " << err;
}
