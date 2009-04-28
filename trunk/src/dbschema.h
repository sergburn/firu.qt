/*
 * dbschema.h
 *
 *  Created on: Apr 28, 2009
 *      Author: burnevsk
 */

#ifndef DBSCHEMA_H_
#define DBSCHEMA_H_

#include <QObject>
#include <QString>
#include <QList>
#include <QLocale>
#include "sqlite3.h"

typedef QLocale::Language Lang;

class DbSchema : public QObject
{
    Q_OBJECT
    
public:
    DbSchema( QObject* parent );
    virtual ~DbSchema();

    class EntryRecord
    {
    public:
        int id;
        QString text;
    };
    
    class TransRecord
    {
    public:
        int id;
        int sid;
        int tid;
        int fmark;
        int rmark;
    };
    
    class TransViewRecord : public TransRecord
    {
    public:
        QString source;
        QString target;
    };
    
    bool open( const QString& dbPath );
    
    QList<EntryRecord> getEntries( Lang lang, const QString& pattern );
    QList<TransViewRecord> getTranslationsByEntry( Lang src, Lang trg, int entry );
    QList<TransViewRecord> getTranslationsByFmark( Lang src, Lang trg, int fmark );
    QList<TransViewRecord> getTranslationsByRmark( Lang src, Lang trg, int rmark );
    
    bool langTableExists( Lang lang );
    bool transTableExists( Lang source, Lang target );
  
    int createLangTable( Lang lang );
    int createTransTable( Lang src, Lang trg );

    int addEntry( Lang lang, const QString& text );
    int addTranslation( Lang src, Lang trg, const QString& source, const QString& target );
    
private:
    DbSchema();
    DbSchema( const DbSchema& );
    
    static int sqlCallback( void*, int, char**, char** );
    int onSqlCallback( int, char**, char** );
    
    int sqlExecute( QString sql );
    bool tableExists( const QString& table );
    
private:
    sqlite3* m_db;
};

#endif /* DBSCHEMA_H_ */
