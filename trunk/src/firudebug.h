#ifndef FIRUDEBUG_H
#define FIRUDEBUG_H

#include <QDebug>
#ifdef FIRU_INTERNAL_SQLITE
#include "sqlite3.h"
#define SQLOK( _e ) ( ((_e) == SQLITE_OK || (_e) == SQLITE_DONE || (_e) == SQLITE_ROW ) ? SQLITE_OK : (_e) )
void LogSqliteError( sqlite3* db, const char* );
#else
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
void LogSqlError( QSqlDatabase& db, const char* );
#endif

#endif // FIRUDEBUG_H
