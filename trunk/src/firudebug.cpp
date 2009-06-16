#include "firudebug.h"
#include <QString>
#include <QDebug>

// ----------------------------------------------------------------------------

void LogSqliteError( sqlite3* db, const char* where )
{
    QString err( sqlite3_errmsg( db ) );
    qDebug() <<
        "SqLite err in " << where <<
        ": err " << sqlite3_errcode( db ) <<
        ", exterr "  << sqlite3_extended_errcode( db ) <<
        ", msg " << err;
}



