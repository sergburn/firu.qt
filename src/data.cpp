#include <QDir>
#include <QDebug>
#include <QTime>
#include <QApplication>

#include "model/dbschema.h"
#include "data.h"

// ----------------------------------------------------------------------------
// TRANSLATION
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// DATA
// ----------------------------------------------------------------------------

Data::Data() 
: m_schema( NULL )
{
    m_schema = new DbSchema();
}

// ----------------------------------------------------------------------------

Data::~Data()
{
}

// ----------------------------------------------------------------------------

bool Data::open()
{
#ifndef SYMBIAN
    QString dbPath = QDir::homePath() + "/.firu/";
    QDir path;
    path.mkpath( dbPath );
#else
    QString dbPath;
#endif
    return m_schema->open( dbPath + "firu.db" );
}

// ----------------------------------------------------------------------------

bool Data::select( Lang src, Lang trg )
{
    int err = m_schema->prepareStatements( src, trg );
    if ( !err )
    {
        m_source_lang = src;
        m_target_lang = trg;
        return true;
    }
    return false;
}

// ----------------------------------------------------------------------------

void Data::getLanguages( Lang& src, Lang& trg ) const
{
    src = m_source_lang;
    trg = m_target_lang;
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
    int numAdded = 0;

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
            int res = addTranslations( src, trg, source, targets );
            if ( res < 0 )
            {
                m_schema->rollback();
                return false;
            }
            numAdded += res;
            ops++;
            if ( ops % 100 == 0 )
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
    qDebug() << "Added" << numAdded << "new translations";
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

Word Data::getWord( qint64 id )
{
    Word word;
    DbSchema::EntryRecord entry;
    int err = m_schema->getEntry( m_source_lang, id, entry );
    if ( !err )
    {
        word = Word( entry.id, entry.text );
    }
    return word;
}

// ----------------------------------------------------------------------------

QList<Word> Data::searchWords( const QString& pattern )
{
    QList<Word> words;
    QList<DbSchema::EntryRecord> entries = m_schema->getEntries( m_source_lang, pattern );
    foreach ( DbSchema::EntryRecord e, entries )
    {
        Word w( e.id, e.text );
        words.append( w );
    }
    return words;
}

// ----------------------------------------------------------------------------

QList<Word> Data::searchTranslations( const QString& pattern )
{
    QList<Word> words;
    QList<DbSchema::TransViewRecord> trans =
            m_schema->getTranslationsByPattern( m_source_lang, m_target_lang, pattern );
    foreach ( DbSchema::TransViewRecord r, trans )
    {
        Word w( r.sid, r.target );
        words.append( w );
    }
    return words;
}

// ----------------------------------------------------------------------------

QList<Translation> Data::getTranslations( const Word& word )
{
    QList<Translation> translations;
    QList<DbSchema::TransViewRecord> trans =
            m_schema->getTranslationsByEntry( m_source_lang, m_target_lang, word.getId() );
    foreach ( DbSchema::TransViewRecord r, trans )
    {
        Translation t( r.id, r.target, (Translation::Mark) r.fmark, (Translation::Mark) r.rmark );
        translations.append( t );

        // change mark
        r.fmark = Translation::ToLearn;
        r.rmark = Translation::ToLearn;
        m_schema->saveTranslationMarks( m_source_lang, m_target_lang, r );
    }
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

