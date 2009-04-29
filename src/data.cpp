#include <QDir>
#include <QDebug>

#include "dbschema.h"
#include "data.h"

// ----------------------------------------------------------------------------



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

bool Data::importDictionary( const QString& fileName )
{
    QFile file( fileName );
    if ( !file.open( QIODevice::ReadOnly | QIODevice::Text ) )
    {
        qDebug() << "Can't open " << fileName << ", err: " << file.errorString();
        return false;
    }

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
//    if ( !isTransExists( source, target ) )
//    {
//        addLanguage( source );
//        addLanguage( target );
//
//        m_schema->createTransTable( source, target );
//    }
    return true;
}
