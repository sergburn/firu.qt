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

    bool start();
    bool next();
    bool execute();

    int error() const;
    void reset();

signals:
    void onQueryProgress();
    void onQueryFinish( Status status );

    Lang source() { return m_srcLang; }
    Lang target() { return m_trgLang; }

protected:
    int addCondition( QString& sql, const char* condition );
    int addSorting( QString& sql, const char* condition );
    int addSet( QString& sql, const char* expr );

    int bindInt( const char* parameter, int value );
    int bindInt64( const char* parameter, qint64 value );
    int bindString( const char* parameter, const QString& value );

    QString createPattern( const QString& text, TextMatch match );

    virtual int bind() {}
    virtual void read() {}

    virtual QString selectBaseSql() const;
    virtual QString updateBaseSql() const = 0;
    virtual QString insertBaseSql() const = 0;
    virtual QString deleteBaseSql() const;

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
};

// ----------------------------------------------------------------------------

class FilteredByPKeyQuery : public Query
{
public:
    FilteredByPKeyQuery( sqlite* db, Lang src, QObject* parent = NULL )
        : Query( db, src, QLocale::C, parent ) {}

    FilteredByPKeyQuery( sqlite* db, Lang src, Lang src, QObject* parent = NULL )
        : Query( db, src, trg, parent ) {}

    void setPrimaryKey( qint64 id )
    {
        m_id = id;
    }

protected:
    void addPrimaryKeyCondition( QString& sql)
    {
        addCondition( "id = :id" );
    }

    int bindPrimaryKey()
    {
        return bindInt64( ":id", m_id );
    }

private:
    qint64 m_id;
};

#endif // QUERY_H
