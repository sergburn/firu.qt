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
