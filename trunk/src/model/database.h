#ifndef DATABASE_H_
#define DATABASE_H_

#include <QObject>
#include <QString>
#include <QHash>
#include "sqlite3.h"

#include "model.h"
#include "query.h"

class Database : public QObject
{
    Q_OBJECT
    
public:
    virtual ~Database();

    static Database* instance();
    static Database* open( const QString& dbPath, QObject* parent );
    
    sqlite3* db();

    bool langTableExists( Lang lang );
    bool transTableExists( LangPair langs );
  
    int createLangTable( Lang lang );
    int createTransTable( LangPair langs );

    static QString getEntryTableName( Lang lang );
    static QString getTransTableName( LangPair langs );

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
    Database( QObject* parent );
    Database( const Database& );
    
    bool doOpen( const QString& dbPath );

    static int sqlCallback( void*, int, char**, char** );
    int onSqlCallback( int, char**, char** );

    int sqlExecute( QString sql );
    int sqlGetTable( const QString& sql );

    bool tableExists( const QString& table );

private:
    sqlite3* m_db;

    typedef QHash<LangPair, Query::Ptr> QueryLangPairHash;
    typedef QHash<QString, QueryLangPairHash> QuerySuperHash;

    QuerySuperHash m_queries;

    int m_transactionLevel;
    int m_transactionError;
};

#endif /* DATABASE_H_ */
