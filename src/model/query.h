#ifndef QUERY_H
#define QUERY_H

#include <QObject>
#include <QSharedPointer>
#include <sqlite3.h>

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

    int error() const;
    void reset();

signals:
    void onQueryProgress();
    void onQueryFinish( Status status );

protected:
    int bindInt( const char* parameter, int value );
    int bindInt64( const char* parameter, qint64 value );
    int bindString( const char* parameter, const QString& value );
    int bindPrimaryKey();

    QString selectBaseSql() const;
    QString countBaseSql() const;
    QString updateBaseSql() const;
    QString deleteBaseSql() const;

protected:
    virtual int bind() { return SQLITE_OK; }
    virtual void read() {}
    virtual QString buildSql() const = 0;

private:
    Query();
    Query( const Query& );

protected:
    sqlite3* m_db;
    sqlite3_stmt* m_stmt;

    QString m_tableName;
    qint64 m_pk;

    QString m_finalSql;
};

#endif // QUERY_H
