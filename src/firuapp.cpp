#include <QDir>
#include <QString>

#include "firuapp.h"
#include "firudebug.h"

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

FiruApp::~FiruApp()
{
}

// ----------------------------------------------------------------------------

bool FiruApp::openDatabase()
{
    QString dbPath;
#ifndef __SYMBIAN32__
    dbPath = QDir::homePath() + "/.firu/";
    QDir path;
    path.mkpath( dbPath );
#endif
    m_database = Database::open( dbPath + "firu.db", this );
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

QStringList FiruApp::getKeypadGroups( Lang lang )
{
    QStringList groups;
    switch ( lang )
    {
        case QLocale::Finnish:
            groups.append( QString( "abc" ) );
            groups.append( QString( "def" ) );
            groups.append( QString( "ghi" ) );
            groups.append( QString( "jkl" ) );
            groups.append( QString( "mno" ) );
            groups.append( QString( "pqrs" ) );
            groups.append( QString( "tuv" ) );
            groups.append( QString( "wxyz" ) );
            groups.append( QString( "\xE4\xF6" ) );
            break;

        case QLocale::Russian:
            break;
        default:
            break;
    }
    return groups;
}

// ----------------------------------------------------------------------------

void FiruApp::startTrainer( Dictionary::Ptr dictionary )
{
    Trainer* trainer = new Trainer( dictionary, get() );
    trainer->start();
}

// ----------------------------------------------------------------------------

TrainerDialog* FiruApp::getTrainerDialog()
{
    FiruApp* self = get();
    if ( !self->m_dlgTrainer )
    {
        self->m_dlgTrainer = new TrainerDialog();
    }
    return self->m_dlgTrainer;
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

Settings::Settings()
{
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

void Settings::setSourceLanguage( Lang src )
{
    m_src = src;
}

// ----------------------------------------------------------------------------

void Settings::setTargetLanguage( Lang trg )
{
    m_trg = trg;
}
