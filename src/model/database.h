#ifndef DATABASE_H_
#define DATABASE_H_

#ifdef __SYMBIAN32__
#include <e32std.h>
#include <f32file.h>
#endif

#include <QObject>
#include <QString>
#include <QHash>
#ifdef FIRU_USE_SQLITE
#include "sqlite3.h"
#else
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#endif

#include "model.h"
#include "query.h"

class Database : public QObject
{
    Q_OBJECT
    
public:
    virtual ~Database();

    static Database* instance();
    static Database* open( const QString& dbPath, QObject* parent );
    
#ifdef FIRU_USE_SQLITE
    sqlite3* db();
#else
    QSqlDatabase& db();
#endif    

    bool langTableExists( Lang lang );
    bool transTableExists( LangPair langs );
  
    int createLangTable( Lang lang );
    int createTransTable( LangPair langs );

    static QString getWordTableName( Lang lang );
    static QString getTransTableName( LangPair langs );

    bool begin();
    bool commit();
    void rollback();
    bool inTransaction() const;

    template <class T>
    static QSharedPointer<T> getQuery( Lang src = QLocale::C )
    {
        return getQuery<T>( LangPair( src, QLocale::C ) );
    }

    template <class T>
    static QSharedPointer<T> getQuery( LangPair langs )
    {
        Database* db = Database::instance();
        Query::Ptr q = db->findQuery( T::staticMetaObject.className(), langs.first, langs.second );
        if ( !q )
        {
            Query::Ptr tq( new T( db, langs ) );
            db->addQuery( tq, langs.first, langs.second );
            q = tq;
        }
        return q.dynamicCast<T>();
    }

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

    Query::Ptr findQuery( const char* className, Lang src, Lang trg );
    void addQuery( Query::Ptr query, Lang src, Lang trg );

private:
#ifdef FIRU_USE_SQLITE
    sqlite3* m_db;
#ifdef __SYMBIAN32__
    RFs iFs;
#endif
#else
    QSqlDatabase m_db;
#endif
    typedef QHash<LangPair, Query::Ptr> QueryLangPairHash;
    typedef QHash<QString, QueryLangPairHash> QuerySuperHash;

    QuerySuperHash m_queries;

    int m_transactionLevel;
    int m_transactionError;

};

#endif /* DATABASE_H_ */
