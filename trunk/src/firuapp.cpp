#include "firuapp.h"

// ----------------------------------------------------------------------------

FiruApp::FiruApp( int argc, char *argv[] )
    : QApplication( argc, argv )
{
    m_settings = new Settings;

    if ( m_settings->getSourceLanguage() == QLocale::C )
    {
        m_settings->setSourceLanguage( QLocale::Finnish );
    }

    if ( m_settings->getTargetLanguage() == QLocale::C )
    {
        m_settings->setTargetLanguage( QLocale::Russian );
    }
}

// ----------------------------------------------------------------------------

bool FiruApp::openDatabase()
{
#ifndef SYMBIAN
    QString dbPath = QDir::homePath() + "/.firu/";
    QDir path;
    path.mkpath( dbPath );
#else
    QString dbPath;
#endif
    m_database = Database::open( dbPath + "firu.db" );
    return ( m_database != NULL );
}

// ----------------------------------------------------------------------------

FiruApp* FiruApp::get()
{
    return dynamic_cast<FiruApp*>( qApp );
}

// ----------------------------------------------------------------------------

Settings& FiruApp::settings()
{
    FiruApp* self = get();
    return *(self->m_settings);
}

// ----------------------------------------------------------------------------

Database& FiruApp::database()
{
    FiruApp* self = get();
    return *(self->m_database);
}

// ----------------------------------------------------------------------------

Lang Settings::getSourceLanguage() const
{
    return m_src;
}

// ----------------------------------------------------------------------------

Lang Settings::getTargetLanguage() const
{
    return m_trg;
}

// ----------------------------------------------------------------------------

void Settings::setSourceLanguage( Lang src ) const
{
    m_src = src;
}

// ----------------------------------------------------------------------------

void Settings::setTargetLanguage( Lang trg ) const
{
    m_trg = trg;
}
