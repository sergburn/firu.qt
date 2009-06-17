#ifndef FIRUDEBUG_H
#define FIRUDEBUG_H

#include "sqlite3.h"
#include <QDebug>

#define SQLOK( _e ) ( ((_e) == SQLITE_OK || (_e) == SQLITE_DONE || (_e) == SQLITE_ROW ) ? SQLITE_OK : (_e) )

void LogSqliteError( sqlite3* db, const char * );

#endif // FIRUDEBUG_H
