#ifndef QUERY_H
#define QUERY_H

#include <QObject>

class Query : public QObject
{
public:

    enum Status
    {
        NONE, RUNNING, SUCCESS, FAILURE
    };

    Status execute();
    int errorCode() const;


signals:
    void onQueryProgress( int progress );
    void onQueryFinish( Status status );

protected:
    Query();
    Query( const Query& );
    Query( DbSchema& db, QString sql );

    int readNextRecord();
    virtual int readRecord() = 0;

protected:
    DbSchema&
    sqlite3_stmt* m_stmt;
};

#endif // QUERY_H
