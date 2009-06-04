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

#include "model.h"
#include "query.h"
#include "wordquery.h"
#include "translationquery.h"

class DbSchema : public QObject
{
    Q_OBJECT
    
public:
    virtual ~DbSchema();

    static DbSchema* instance();
    static DbSchema* open( const QString& dbPath, QObject* parent );
    
//    int getEntry( Lang lang, qint64 id, EntryRecord& record );
//    int getEntry( Lang lang, const QString& text, EntryRecord& record );
//    QList<EntryRecord> getEntries( Lang lang, const QString& pattern );
//
//    QList<TransViewRecord> getTranslationsByPattern( Lang src, Lang trg, const QString& pattern );
//    QList<TransViewRecord> getTranslationsByEntry( Lang src, Lang trg, qint64 sid );
//    QList<TransViewRecord> getTranslationsByFmark( Lang src, Lang trg, int fmark );
//    QList<TransViewRecord> getTranslationsByRmark( Lang src, Lang trg, int rmark );
//
//    int saveTranslationMarks( Lang src, Lang trg, TransViewRecord r );
    
    bool langTableExists( Lang lang );
    bool transTableExists( Lang source, Lang target );
  
    int createLangTable( Lang lang );
    int createTransTable( Lang src, Lang trg );

    int addEntry( Lang lang, const QString& text, qint64& id );
    int addTranslation( Lang src, Lang trg, qint64 sid, const QString& text, qint64& id );

    bool begin();
    bool commit();
    void rollback();

    QSharedPointer<WordQuery> getWordQuery( Lang src );
    QSharedPointer<TranslationQuery> getTranslationQuery( Lang src, Lang trg );

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

//    QList<EntryRecord> getAllEntryRecords( sqlite3_stmt* stmt );
//    int nextEntryRecord( sqlite3_stmt* stmt, EntryRecord& record );
//    void readEntryRecord( sqlite3_stmt* stmt, EntryRecord& record );
//
//    QList<TransViewRecord> getAllTransViewRecords( sqlite3_stmt* stmt );
//    int nextTransViewRecord( sqlite3_stmt* stmt, TransViewRecord& record );
//    void readTransViewRecord( sqlite3_stmt* stmt, TransViewRecord& record );
    
    void freeStatements();
    
    void LogSqliteError( const char * );

    template <class T> QSharedPointer<T> getQuery( const char* className, Lang src = QLocale::C, Lang trg = QLocale::C );
    QWeakPointer<Query> findQuery( const char* className, Lang src = QLocale::C, Lang trg = QLocale::C );

private:
    sqlite3* m_db;
    QList<QWeakPointer<Query> > m_queries;
};

#endif /* DBSCHEMA_H_ */
