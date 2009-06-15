#include "dictionary.h"
#include "database.h"

// ----------------------------------------------------------------------------

Dictionary::Dictionary( LangPair langs )
    : m_langs( langs )
{
}

// ----------------------------------------------------------------------------

bool Dictionary::open()
{
    // Create needed tables
    if ( !Database::langTableExists( source() ) )
    {
        int err = Database::createLangTable( source() );
        if ( err ) return false;
    }
    if ( !Database::transTableExists( source(), target() ) )
    {
        int err = Database::createTransTable( source(), target() );
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
        return false;
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
                dict = new Dictionary( LangPair( src, trg ) );
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
            int res = dp->addWord( source, targets );
            if ( res < 0 )
            {
                db->rollback();
                ok = false;
                break;
            }
            numAdded += res;
            ops++;
            if ( ops % 100 == 0 )
            {
                if ( !db->commit() && !db->begin() )
                {
                    ok = false;
                    break;
                }

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
    return ok;
}

// ----------------------------------------------------------------------------

int Dictionary::addWord( const QString& source, const QStringList& targets )
{
    if ( source.length() == 0 || targets.count() == 0 )
    {
        return 0;
    }
    int err = 0;

    // Add source
    Word::Ptr word = findWord( source, FullMatch );
    if ( !word )
    {
        qDebug() << "newWord: " << source;
        word = new Word( source, source() );
        word->save();
    }

    int numAddedTranslations = 0;
    foreach( QString target, targets )
    {
        if ( word->addTranslation( target, target() ) )
        {
            qDebug() << "newTranslation: " << target;
            numAddedTranslations++;
        }
    }

    word->save();

    return numAdded;
}

// ----------------------------------------------------------------------------

Translation::List Dictionary::findTranslations( qint64 sid )
{
    return Translation::findBySourceEntry( sid, m_langs );
}

// ----------------------------------------------------------------------------

Word::List findWords( const QString& pattern, TextMatch match, int limit )
{
    return Word::find( pattern, source(), match, limit );
}

// ----------------------------------------------------------------------------

Translation::List findTranslations( const QString& pattern, TextMatch match, int limit )
{
    return Translation::find( pattern, m_langs, match, limit );
}
