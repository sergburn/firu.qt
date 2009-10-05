#include "firudebug.h"
#include <QString>
#include <QDebug>

// ----------------------------------------------------------------------------

#ifdef FIRU_USE_SQLITE
void LogSqliteError( sqlite3* db, const char* where )
{
    QString err( sqlite3_errmsg( db ) );
    qDebug() <<
        "SqLite err in " << where <<
        ": err " << sqlite3_errcode( db ) <<
        ", exterr "  << sqlite3_extended_errcode( db ) <<
        ", msg " << err;
}
#else
void LogSqlError( QSqlDatabase& db, const char* where )
{
    QSqlError e = db.lastError();
    qDebug() 
        << "QtSql err in" << where << ": n" << e.number()
        << "t" << e.type() << "m" << e.text();
}
#endif



