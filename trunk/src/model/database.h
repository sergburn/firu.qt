/*
 * dbschema.h
 *
 *  Created on: Apr 28, 2009
 *      Author: burnevsk
 */

#ifndef DATABASE_H_
#define DATABASE_H_

#include <QObject>
#include <QString>
#include <QList>
#include <QLocale>
#include "sqlite3.h"

#include "model.h"
#include "query.h"
#include "wordquery.h"
#include "translationquery.h"

class Database : public QObject
{
    Q_OBJECT
    
public:
    virtual ~Database();

    static Database* instance();
    static Database* open( const QString& dbPath, QObject* parent );
    
    bool langTableExists( Lang lang );
    bool transTableExists( Lang source, Lang target );
  
    int createLangTable( Lang lang );
    int createTransTable( Lang src, Lang trg );

    bool begin();
    bool commit();
    void rollback();
    bool inTransaction() const;

    Query::Ptr findQuery( const char* className, Lang src = QLocale::C, Lang trg = QLocale::C );
    void addQuery( Query::Ptr query );

signals:
    void onSqlProgress();
    void onTransactionFinish( bool success );

private:
    Database();
    Database( const Database& );
    
    static int sqlCallback( void*, int, char**, char** );
    int onSqlCallback( int, char**, char** );

    int sqlExecute( QString sql );
    int sqlGetTable( const QString& sql );

    bool tableExists( const QString& table );

    void LogSqliteError( const char * );

private:
    sqlite3* m_db;
    QList<Query::Ptr> m_queries;
    int m_transactionLevel;
    int m_transactionError;
};

#endif /* DATABASE_H_ */
