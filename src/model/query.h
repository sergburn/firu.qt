#ifndef QUERY_H
#define QUERY_H

#include <QObject>
#include <QSharedPointer>
#include <sqlite3.h>

#include "model.h"

class Database;

class Query : public QObject
{
public:
    typedef QSharedPointer<Query> Ptr;

    Query( Database* db, Lang src, QObject* parent = NULL );
    Query( Database* db, LangPair langs, QObject* parent = NULL );
    ~Query();

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

    Lang source() const { return m_srcLang; }
    Lang target() const { return m_trgLang; }
    LangPair getLangs() const { return LangPair( m_srcLang, m_trgLang ); }

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

private slots:
    void queryProgress();

protected:
    sqlite3* m_db;
    sqlite3_stmt* m_stmt;

    Lang m_srcLang;
    Lang m_trgLang;

    QString m_tableName;
    qint64 m_pk;
};

#endif // QUERY_H
