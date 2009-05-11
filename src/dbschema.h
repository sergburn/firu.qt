/*
 * dbschema.h
 *
 *  Created on: Apr 28, 2009
 *      Author: burnevsk
 */

#ifndef DBSCHEMA_H_
#define DBSCHEMA_H_

#include <QObject>
#include <QString>
#include <QList>
#include <QLocale>
#include "sqlite3.h"

typedef QLocale::Language Lang;

class DbSchema : public QObject
{
    Q_OBJECT
    
public:
    DbSchema( QObject* parent );
    virtual ~DbSchema();

    class EntryRecord
    {
    public:
        EntryRecord() : id( 0 ) {};
        qint64 id;
        QString text;
    };
    
    class TransRecord
    {
    public:
        TransRecord() : id( 0 ), sid( 0 ), fmark( 0 ), rmark( 0 ) {};
        qint64 id;
        qint64 sid;
        QString target;
        int fmark;
        int rmark;
    };
    
    class TransViewRecord : public TransRecord
    {
    public:
        QString source;
    };
    
    bool open( const QString& dbPath );
    
    int getEntry( Lang lang, qint64 id, EntryRecord& record );
    int getEntry( Lang lang, const QString& text, EntryRecord& record );
    QList<EntryRecord> getEntries( Lang lang, const QString& pattern );
    
    QList<TransViewRecord> getTranslationsByPattern( Lang src, Lang trg, const QString& pattern );
    QList<TransViewRecord> getTranslationsByEntry( Lang src, Lang trg, qint64 sid );
    QList<TransViewRecord> getTranslationsByFmark( Lang src, Lang trg, int fmark );
    QList<TransViewRecord> getTranslationsByRmark( Lang src, Lang trg, int rmark );

    int saveTranslationMarks( Lang src, Lang trg, TransViewRecord r );
    
    bool langTableExists( Lang lang );
    bool transTableExists( Lang source, Lang target );
  
    int createLangTable( Lang lang );
    int createTransTable( Lang src, Lang trg );

    int addEntry( Lang lang, const QString& text, qint64& id );
    int addTranslation( Lang src, Lang trg, qint64 sid, const QString& text, qint64& id );

    bool begin();
    bool commit();
    void rollback();

    int prepareStatements( Lang src, Lang trg );

signals:
    void onLongOpProgress();

private:
    DbSchema();
    DbSchema( const DbSchema& );
    
    static int sqlCallback( void*, int, char**, char** );
    int onSqlCallback( int, char**, char** );
    
    static int sqlProgress( void* );
    int onSqlProgress();

    int sqlExecute( QString sql );
    int sqlGetTable( const QString& sql );

    bool tableExists( const QString& table );

    QList<EntryRecord> getAllEntryRecords( sqlite3_stmt* stmt );
    int nextEntryRecord( sqlite3_stmt* stmt, EntryRecord& record );
    void readEntryRecord( sqlite3_stmt* stmt, EntryRecord& record );

    QList<TransViewRecord> getAllTransViewRecords( sqlite3_stmt* stmt );
    int nextTransViewRecord( sqlite3_stmt* stmt, TransViewRecord& record );
    void readTransViewRecord( sqlite3_stmt* stmt, TransViewRecord& record );
    
    void freeStatements();
    
    void LogSqliteError( const char * );
    
private:
    sqlite3* m_db;
    
    sqlite3_stmt* m_insertEntry;
    sqlite3_stmt* m_insertTrans;
    sqlite3_stmt* m_updateTrans;
    sqlite3_stmt* m_selectEntryById;
    sqlite3_stmt* m_selectEntriesByPattern;
    sqlite3_stmt* m_selectTransByPattern;
    sqlite3_stmt* m_selectTransById;
    sqlite3_stmt* m_selectTransBySid;
    sqlite3_stmt* m_selectTransByText;
    sqlite3_stmt* m_selectTransByFmark;
    sqlite3_stmt* m_selectTransByRmark;
    Lang m_lastSrc, m_lastTrg;
};

#endif /* DBSCHEMA_H_ */
