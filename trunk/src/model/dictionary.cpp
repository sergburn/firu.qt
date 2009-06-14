#include "dictionary.h"
#include "database.h"

// ----------------------------------------------------------------------------

Dictionary::Dictionary( LangPair langs )
    : m_srcLang( langs.first ), m_trgLang( langs.second )
{
}

// ----------------------------------------------------------------------------

bool Dictionary::open()
{
    // Create needed tables
    if ( !Database::langTableExists( m_srcLang ) )
    {
        int err = Database::createLangTable( m_srcLang );
        if ( err ) return false;
    }
    if ( !Database::transTableExists( m_srcLang, m_trgLang ) )
    {
        int err = Database::createTransTable( m_srcLang, m_trgLang );
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
        word = new Word( source, m_srcLang );
        word->save();
    }

    int numAddedTranslations = 0;
    foreach( QString target, targets )
    {
        if ( word->addTranslation( target, m_trgLang ) )
        {
            qDebug() << "newTranslation: " << target;
            numAddedTranslations++;
        }
    }

    word->save();

    return numAdded;
}
