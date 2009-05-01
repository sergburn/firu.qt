#include <QDir>
#include <QDebug>
#include <QTime>
#include <QApplication>

#include "dbschema.h"
#include "data.h"

// ----------------------------------------------------------------------------

Data::Data() 
: m_schema( NULL )
{
    m_schema = new DbSchema( this );
}

// ----------------------------------------------------------------------------

Data::~Data()
{
}

// ----------------------------------------------------------------------------

bool Data::open()
{
    QString dbPath = QDir::homePath() + "/.firu/";
    QDir path;
    path.mkpath( dbPath );
    return m_schema->open( dbPath + "firu.db" );
}

// ----------------------------------------------------------------------------

bool Data::select( Lang src, Lang trg )
{
    return ( 0 == m_schema->prepareStatements( src, trg ) );
}

// ----------------------------------------------------------------------------

bool Data::importDictionary( const QString& fileName )
{
    QFile file( fileName );
    if ( !file.open( QIODevice::ReadOnly | QIODevice::Text ) )
    {
        qDebug() << "Can't open " << fileName << ", err: " << file.errorString();
        return false;
    }

    qint64 total = file.size();

    Lang src = QLocale::C;
    Lang trg = QLocale::C;
    QString source;
    QStringList targets;

    int ops = 0;
    m_schema->begin();

    QTextStream in( &file );
    while ( !in.atEnd() )
    {
        QString line = in.readLine();

        if ( line.startsWith('#') )
        {
            if ( line.startsWith( "#INDEX_LANGUAGE" ) )
            {
                src = QLocale::Finnish; // getLang( line );
            }
            else if ( line.startsWith( "#CONTENTS_LANGUAGE" ) )
            {
                trg = QLocale::Russian; // getLang( line );
            }
        }
        else if ( !line.isEmpty() && !line[0].isSpace() )
        {
            source = line;
        }
        else if ( line.contains("[trn]") )
        {
            int start = line.indexOf("[trn]") + QString("[trn]").length();
            int end = line.indexOf("[/trn]");
            if ( end > start )
            {
                targets.append( line.mid( start, end - start ) );
            }
        }
        else if ( line.trimmed().isEmpty() )
        {
            bool ok = addTranslations( src, trg, source, targets );
            if ( !ok )
            {
                m_schema->rollback();
                return false;
            }
            ops++;
            if ( ops % 10 == 0 )
            {
                if ( m_schema->commit() )
                   m_schema->begin();
                else
                    return false;

                double prog = in.pos() * 100.0 / total;
                emit progress( prog );
                QCoreApplication::processEvents();
            }

            source.clear();
            targets.clear();
        }
    }
    m_schema->commit();
    return true;
}

// ----------------------------------------------------------------------------

void Data::setSourceLanguage( Lang lang )
{
    if ( m_source_lang != lang && isLangExists( lang ) )
    {
        m_source_lang = lang;
        emit sourceLangChanged( lang );
    }
}

// ----------------------------------------------------------------------------

void Data::setTargetLanguage( Lang lang )
{
    if ( m_target_lang != lang && isLangExists( lang ) )
    {
        m_target_lang = lang;
        emit targetLangChanged( lang );
    }
}

// ----------------------------------------------------------------------------

void Data::reverseLanguages()
{
    
}

// ----------------------------------------------------------------------------

QList<Word> Data::searchWords( const QString& /*pattern*/ )
{
    QList<Word> words;
    return words;
}

// ----------------------------------------------------------------------------

QList<Translation> Data::getTranslations( const Word& /*word*/ )
{
    QList<Translation> translations;
    return translations;
}

// ----------------------------------------------------------------------------

bool Data::isLangExists( Lang lang )
{
    return m_schema->langTableExists( lang );
}

// ----------------------------------------------------------------------------

bool Data::isTransExists( Lang source, Lang target )
{
    return m_schema->transTableExists( source, target );
}

// ----------------------------------------------------------------------------

bool Data::addLanguage( Lang lang )
{
    if ( !isLangExists( lang ) )
    {
        return 0 == m_schema->createLangTable( lang );
    }
    else
    {
        return true;
    }
}

// ----------------------------------------------------------------------------

bool Data::addTranslations( Lang src, Lang trg, const QString& source, const QStringList& targets )
{
    if ( source.length() == 0 || targets.count() == 0 )
    {
        return true;
    }
    int err = 0;

    // Create needed tables
    if ( !m_schema->langTableExists( src ) )
    {
        err = m_schema->createLangTable( src );
        if ( err ) return false;
    }
    if ( !m_schema->transTableExists( src, trg ) )
    {
        err = m_schema->createTransTable( src, trg );
        if ( err ) return false;
    }
    
//    static int ops = 0;
//    static int getEntryTime = 0;
//    static int addEntryTime = 0;
//    static int getTransTime = 0;
//    static int addTransTime = 0;

    // Prepare statements
    err = m_schema->prepareStatements( src, trg );
    if ( err ) return false;

//    QTime bench;
//    bench.start();
//    ops++;

    // Add source
    DbSchema::EntryRecord entry;
    err = m_schema->getEntry( src, source, entry );
//    getEntryTime = (getEntryTime * ( ops-1 ) + bench.restart() ) / ops;
    if ( !entry.id )
    {
        err = m_schema->addEntry( src, source, entry.id );
        if ( err ) return false;
//        addEntryTime = (addEntryTime * ( ops-1 ) + bench.restart() ) / ops;
    }
    
    // Add translation
    bool exists = false;
    QList<DbSchema::TransViewRecord> translations = m_schema->getTranslationsByEntry( src, trg, entry.id );
//    getTransTime = (getTransTime * ( ops-1 ) + bench.restart() ) / ops;
    foreach ( const QString& target, targets )
    {
        qDebug() << "addTranslation: " << source << "<->" << target;
        foreach ( DbSchema::TransViewRecord trans, translations )
        {
            if ( trans.target == target )
            {
                exists = true;
                break;
            }
        }
        if ( !exists )
        {
            qint64 tid = 0;
            err = m_schema->addTranslation( src, trg, entry.id, target, tid );
            if ( err ) return false;
    //        addTransTime = (addTransTime * ( ops-1 ) + bench.restart() ) / ops;
        }
    }

//    qDebug() << "Times:" <<
//            "getEntry" << getEntryTime <<
//            "addEntry" << addEntryTime <<
//            "getTrans" << getTransTime <<
//            "addTrans" << addTransTime;
    return true;
}
