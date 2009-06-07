#include "dictionary.h"
#include "database.h"

// ----------------------------------------------------------------------------

Vocabulary::Vocabulary( Lang src )
    : m_srcLang( src )
{
}

// ----------------------------------------------------------------------------

Translations::Translations( Lang src, Lang trg )
    : m_srcLang( src ), m_trgLang( trg )
{
}

// ----------------------------------------------------------------------------

Dictionary::Dictionary( Lang src, Lang trg )
    : m_vocab( src ), m_trans( src, trg )
{
}

// ----------------------------------------------------------------------------

bool Data::import( const QString& fileName )
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
    Database* db = Database::instance();
    db->begin();

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
                db->rollback();
                return false;
            }
            numAdded += res;
            ops++;
            if ( ops % 100 == 0 )
            {
                if ( db->commit() )
                   db->begin();
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

int Data::addTranslations( Lang src, Lang trg, const QString& source, const QStringList& targets )
{
    if ( source.length() == 0 || targets.count() == 0 )
    {
        return 0;
    }
    int err = 0;

    // Create needed tables
    if ( !m_schema->langTableExists( src ) )
    {
        err = m_schema->createLangTable( src );
        if ( err ) return -1;
    }
    if ( !m_schema->transTableExists( src, trg ) )
    {
        err = m_schema->createTransTable( src, trg );
        if ( err ) return -1;
    }

//    static int ops = 0;
//    static int getEntryTime = 0;
//    static int addEntryTime = 0;
//    static int getTransTime = 0;
//    static int addTransTime = 0;

//    QTime bench;
//    bench.start();
//    ops++;

    // Add source
    Word::Ptr wp = Word::find( src, source, FullMatch );
    if ( !wp )
    {
        wp = new Word( source, src );
    }
    else
    {
        wp->loadTranslations();
    }

    foreach( QString target, targets )
    {
        wp->addTranslation( target, trg );
    }

    if ( !entry.id )
    {
        err = m_schema->addEntry( src, source, entry.id );
        if ( err ) return -1;
//        addEntryTime = (addEntryTime * ( ops-1 ) + bench.restart() ) / ops;
    }

    // Add translation
    bool exists = false;
    QList<DbSchema::TransViewRecord> translations = m_schema->getTranslationsByEntry( src, trg, entry.id );
//    getTransTime = (getTransTime * ( ops-1 ) + bench.restart() ) / ops;
    int numAdded = 0;
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
            if ( err ) return -1;
            numAdded++;
    //        addTransTime = (addTransTime * ( ops-1 ) + bench.restart() ) / ops;
        }
    }

//    qDebug() << "Times:" <<
//            "getEntry" << getEntryTime <<
//            "addEntry" << addEntryTime <<
//            "getTrans" << getTransTime <<
//            "addTrans" << addTransTime;
    return numAdded;
}
