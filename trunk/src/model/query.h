#ifndef QUERY_H
#define QUERY_H

#include <QObject>
#include <QSharedPointer>
#ifdef FIRU_USE_SQLITE
#include <sqlite3.h>
#else
#include <QSqlQuery>
#include <QSqlDatabase>
#endif

#include "model.h"

class Database;

class Query : public QObject
{
    Q_OBJECT

public:
    typedef QSharedPointer<Query> Ptr;

    Query( Database* db );
    virtual ~Query();

    enum Status
    {
        NONE, RUNNING, SUCCESS, FAILURE
    };

    void setPrimaryKey( qint64 id );

    bool start();
    bool next();
    virtual bool execute();

    void reset();

signals:
    void onQueryProgress();
    void onQueryFinish( Status status );

protected:
    int bindInt( const char* parameter, int value );
    int bindUint( const char* parameter, uint value );
    int bindInt64( const char* parameter, qint64 value );
    int bindUint64( const char* parameter, quint64 value );
    int bindString( const char* parameter, const QString& value );
    int bindPrimaryKey();

    QString selectBaseSql() const;
    QString countBaseSql() const;
    QString updateBaseSql() const;
    QString deleteBaseSql() const;

protected:
    virtual int bind() { return 0; }
    virtual void read() {}
    virtual QString buildSql() const = 0;

private:
    Query();
    Query( const Query& );

protected:
#ifdef FIRU_USE_SQLITE
    sqlite3* m_db;
    sqlite3_stmt* m_stmt;
#else
    QSqlQuery m_query;
    QSqlDatabase& m_db;
#endif

    QString m_tableName;
    qint64 m_pk;

    QString m_finalSql;
};

#endif // QUERY_H
