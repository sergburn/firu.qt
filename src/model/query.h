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

    Query( QObject* parent, sqlite3* db, Lang src = QLocale::C, Lang trg = QLocale::C );
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

    virtual int prepare() = 0;
    virtual int bind() = 0;
    virtual void read() = 0;

    virtual void doReset() {}

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
};

#endif // QUERY_H
