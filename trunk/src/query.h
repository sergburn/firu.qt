#ifndef QUERY_H
#define QUERY_H

#include <QObject>

typedef QLocale::Language Lang;

class Query : public QObject
{
public:
    Query( sqlite3* db, Lang src = QLocale::C, Lang trg = QLocale::C );
    ~Query();

//    enum Status
//    {
//        NONE, RUNNING, SUCCESS, FAILURE
//    };
//
//    Status execute();
//    int errorCode() const;
    start();

//signals:
//    void onQueryProgress( int progress );
//    void onQueryFinish( Status status );

    Lang source() { return m_srcLang; }
    Lang target() { return m_trgLang; }

protected:
    int addCondition( QString& sql, const char* condition );
    int bindInt( const char* parameter, int value );
    int bindInt64( const char* parameter, qint64 value );
    int bindString( const char* parameter, const QString& value );
    int addSorting( QString& sql, const char* condition );

    virtual int prepare() = 0;
    virtual int bind() = 0;
    virtual void read() = 0;

private:
    Query();
    Query( const Query& );

protected:
    sqlite3* m_db;
    sqlite3_stmt* m_stmt;
    bool m_sortAscending;
    Lang m_srcLang;
    Lang m_trgLang;
};

#endif // QUERY_H
