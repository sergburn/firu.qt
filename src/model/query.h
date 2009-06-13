#ifndef QUERY_H
#define QUERY_H

#include <QObject>
#include <QSharedPointer>
#include <sqlite3.h>

#include "model.h"

class Query : public QObject
{
public:
    typedef QSharedPointer<Query> Ptr;

    Query( sqlite3* db, Lang src, QObject* parent = NULL );
    Query( sqlite3* db, Lang src, Lang trg, QObject* parent = NULL );
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

signals:
    void onQueryProgress();
    void onQueryFinish( Status status );

    Lang source() { return m_srcLang; }
    Lang target() { return m_trgLang; }

protected:
    void addCondition( QString& sql, const char* condition );
    void addSorting( QString& sql, const char* condition );
    void addSet( QString& sql, const char* expr );
    void addPrimaryKeyCondition( QString& sql );

    int bindInt( const char* parameter, int value );
    int bindInt64( const char* parameter, qint64 value );
    int bindString( const char* parameter, const QString& value );
    int bindPrimaryKey();

    QString createPattern( const QString& text, TextMatch match );

    virtual int bind() {}
    virtual void read() {}

    virtual QString selectBaseSql() const;
    virtual QString updateBaseSql() const;
    virtual QString deleteBaseSql() const;
    virtual QString insertBaseSql() const = 0;
    virtual QString buildSql() const = 0;

private:
    Query();
    Query( const Query& );

private slots:
    void queryProgress();

protected:
    sqlite3* m_db;
    sqlite3_stmt* m_stmt;
    bool m_sortAscending;
    Lang m_srcLang;
    Lang m_trgLang;

    int m_conditions;
    int m_sets;
    QString m_tableName;
    qint64 m_pk;
};

#endif // QUERY_H
