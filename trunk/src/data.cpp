#include <QDir>
#include <QDebug>
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

    QTextStream in( &file );
    while ( !in.atEnd() )
    {
        QString line = in.readLine();
        qDebug() << line;

        if ( line.startsWith( "#INDEX_LANGUAGE" ) )
        {
            src = QLocale::Finnish; // getLang( line );
        }
        else if ( line.startsWith( "#CONTENTS_LANGUAGE" ) )
        {
            trg = QLocale::Russian; // getLang( line );
        }
        else if ( line.contains("[trn]") )
        {
            int start = line.indexOf("[trn]") + QString("[trn]").length();
            int end = line.indexOf("[/trn]");
            if ( end > start )
            {
                QString target = line.mid( start, end - start );
                addTranslation( src, trg, source, target );
            }
        }
        else if ( !line[0].isSpace() && line[0] != '#' )
        {
            source = line;
        }

        double prog = in.pos() * 100.0 / total;
        qDebug() << "Stream pos " << in.pos() << ", progress " << prog;
        emit progress( prog );
        QCoreApplication::processEvents();
    }

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

bool Data::addTranslation( Lang src, Lang trg, const QString& source, const QString& target )
{
    qDebug() << "addTranslation: " << source << "<->" << target;
    int err = SQLITE_OK;

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
    
    // Prepare statements
    err = m_schema->prepareStatements( src, trg );
    if ( err ) return false;

    // Add source
    DbSchema::EntryRecord entry;
    err = m_schema->getEntry( src, source, entry );
    if ( !entry.id )
    {
        err = m_schema->addEntry( src, source, entry.id );
        if ( err ) return false;
    }
    
    // Add translation
    qint64 tid = 0;
    err = m_schema->addTranslation( src, trg, entry.id, target, tid );
    return err;
}
