#include <QFile>
#include <QStringList>
#include <QCoreApplication>

#include "dictionary.h"
#include "database.h"
#include "translationquery.h"
#include "../firudebug.h"

// ----------------------------------------------------------------------------

Dictionary::Dictionary( LangPair langs, QObject* parent )
    : QObject( parent ), m_langs( langs )
{
}

// ----------------------------------------------------------------------------

bool Dictionary::open()
{
    Database* db = Database::instance();
    // Create needed tables
    if ( !db->langTableExists( source() ) )
    {
        int err = db->createLangTable( source() );
        if ( err ) return false;
    }
    if ( !db->transTableExists( m_langs ) )
    {
        int err = db->createTransTable( m_langs );
        if ( err ) return false;
    }
    return true;
}

// ----------------------------------------------------------------------------

Dictionary::Ptr Dictionary::import( const QString& fileName )
{
    QFile file( fileName );
    if ( !file.open( QIODevice::ReadOnly | QIODevice::Text ) )
    {
        qDebug() << "Can't open " << fileName << ", err: " << file.errorString();
        return Dictionary::Ptr();
    }

    qint64 total = file.size();
    int numWords = 0;
    int numTranslations = 0;

    Lang src = QLocale::C;
    Lang trg = QLocale::C;
    QString source;
    QStringList targets;

    int ops = 0;
    Database* db = Database::instance();
    db->begin();

    Dictionary::Ptr dict;

    bool ok = true;
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

            if ( src != QLocale::C && trg != QLocale::C )
            {
                dict = Dictionary::Ptr( new Dictionary( LangPair( src, trg ) ) );
                ok = dict->open();
                if ( !ok ) break;
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
        else if ( line.trimmed().isEmpty() && dict )
        {
            int res = dict->addWord( source, targets );
            if ( res < 0 )
            {
                db->rollback();
                ok = false;
                break;
            }
            numWords++;
            numTranslations += res - 1;
            ops++;
            if ( ops % 100 == 0 )
            {
                if ( !db->commit() && !db->begin() )
                {
                    ok = false;
                    break;
                }

                double prog = in.pos() * 100.0 / total;
                emit dict->progress( prog );
                QCoreApplication::processEvents();
            }

            source.clear();
            targets.clear();
        }
    }
    db->commit();
    qDebug() << "Added" << numWords << "new words and" << numTranslations << "new translations";
    return dict;
}

// ----------------------------------------------------------------------------

int Dictionary::addWord( const QString& text, const QStringList& translations )
{
    if ( text.length() == 0 || translations.count() == 0 )
    {
        return 0;
    }

    int numAdded = 0;

    // Add source
    Word::Ptr word;
    if ( !Word::exists( text, source() ) )
    {
        qDebug() << "newWord: " << text;
        word = Word::Ptr( new Word( text, source() ) );
        numAdded++;
    }

    foreach( QString t, translations )
    {
        if ( word->addTranslation( t, target() ) )
        {
            qDebug() << "newTranslation: " << t;
            numAdded++;
        }
    }

    if ( word->save() )
        return numAdded;
    else
        return 0;
}

// ----------------------------------------------------------------------------

Translation::List Dictionary::findTranslations( qint64 sid )
{
    return Translation::findBySourceEntry( sid, m_langs );
}

// ----------------------------------------------------------------------------

Word::List Dictionary::findWords( const QString& pattern, TextMatch match, int limit )
{
    return Word::find( pattern, source(), match, limit );
}

// ----------------------------------------------------------------------------

int Dictionary::count( const QString& pattern )
{
    return Word::count( pattern, source() );
}

// ----------------------------------------------------------------------------

Translation::List Dictionary::findTranslations( const QString& pattern, TextMatch match, int limit )
{
    return Translation::find( pattern, m_langs, match, limit );
}

// ----------------------------------------------------------------------------

bool Dictionary::setWordToLearn( qint64 /*id*/ )
{
//        // drop marks for these translations
//        foreach( Translation t, m_translations )
//        {
//            t.fmark().restart();
//            t.rmark().restart();
//        }
//        Translation::saveMarks( m_translations, m_id );
    return false;
}

// ----------------------------------------------------------------------------

bool Dictionary::addToUserDict( qint64 sid )
{
    return Translation::addToUserDict( sid, m_langs );
}

// ----------------------------------------------------------------------------

bool Dictionary::clearLearningSet()
{
    ResetMarksQuery::Ptr qry = Database::getQuery<ResetMarksQuery>( m_langs );
    if ( qry )
    {
        return qry->execute();
    }
    return false;
}

// ----------------------------------------------------------------------------

Exercise::Ptr Dictionary::createReverseExercise()
{
    return Exercise::generate( m_langs );
}
