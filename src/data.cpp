#include <QDir>
#include <QDebug>

#include "data.h"

Data::Data()
{
}

Data::~Data()
{
    if ( m_db )
    {
        sqlite3_close( m_db );
    }
}

bool Data::open()
{
    QString dbPath = QDir::homePath() + "/.firu/dicts.db";
    QDir path;
    path.mkpath( dbPath );

    int err = sqlite3_open( (const char*) dbPath.constData(), &m_db );
    if ( err == SQLITE_OK )
    {
        return true;
    }
    else if ( m_db )
    {
        QString msg( sqlite3_errmsg( m_db ) );
        qDebug() << msg;
        sqlite3_close( m_db );
        return false;
    }
    else
    {
        return false;
    }
}

void Data::setSourceLanguage( Lang lang )
{
    if ( m_source_lang != lang && isLangExists( lang ) )
    {
        m_source_lang = lang;
        emit sourceLangChanged( lang );
    }
}

void Data::setTargetLanguage( Lang lang )
{
    if ( m_target_lang != lang && isLangExists( lang ) )
    {
        m_target_lang = lang;
        emit targetLangChanged( lang );
    }
}

void Data::reverseLanguages()
{
    
}

QString getLangTableName( Lang lang )
{
    return QString( "entries_" ) + QLocale::languageToString( lang );
}

QString getTransTableName( Lang source, Lang target )
{
    return QString( "trans_" ) + 
        QLocale::languageToString( source ) + "_" +
        QLocale::languageToString( target );
}

bool Data::isLangExists( Lang lang )
{
    return isTableExists( getLangTableName( lang ) );
}

bool Data::isTransExists( Lang source, Lang target )
{
    return isTableExists( getTransTableName( source, target ) );
}

bool Data::addLanguage( Lang lang )
{
    if ( !isLangExists( lang ) )
    {
        return 0 == createLangTable( QLocale::languageToString( lang ) );
    }
    else
    {
        return true;
    }
}

bool Data::addTranslation( Lang source, Lang target )
{
    if ( !isTransExists( source, target ) )
    {
        addLanguage( source );
        addLanguage( target );
        
        createTransTable( 
            QLocale::languageToString( source ), 
            QLocale::languageToString( target ) );
    }
    return true;
}

bool Data::isTableExists( const QString& table )
{
    return false;
}

int Data::createLangTable( const QString& lang )
{
    return SQLITE_ABORT;
}

int Data::createTransTable( const QString& src, const QString& trg )
{
    return SQLITE_ABORT;
}
