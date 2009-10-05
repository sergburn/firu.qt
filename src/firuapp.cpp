#include <QDir>
#include <QFile>
#include <QString>
#include <QDesktopWidget>

#include "firuapp.h"
#include "firudebug.h"

// ----------------------------------------------------------------------------

FiruApp::FiruApp( int argc, char *argv[] )
    : QApplication( argc, argv ),
    m_settings( NULL ), m_database( NULL ), 
    m_trainer( NULL ), m_dlgTrainer( NULL )
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
    
    QRect desk = QApplication::desktop()->screenGeometry();
    qDebug() << "Screen:" << desk;
    
    QString style = desk.height() > 320 ? ":/files/khaki.qss" : ":/files/khaki_small.qss"; 
    QFile styleSheet( style );
    if ( !styleSheet.open( QIODevice::ReadOnly ) ) 
    {
        qWarning() << "Unable to open " << style;
        return;
    }
    setStyleSheet( styleSheet.readAll() );
}

// ----------------------------------------------------------------------------

FiruApp::~FiruApp()
{
    delete m_trainer;
    delete m_dlgTrainer;
}

// ----------------------------------------------------------------------------

bool FiruApp::openDatabase()
{
    qDebug() << "Home path is:" << QDir::homePath();

    QDir dir;
    QString dbPath;
#ifdef __SYMBIAN32__
    dbPath = "e:\\data\\.firu\\";
    if ( !dir.mkpath( dbPath ) )
    {
        dbPath = "c:\\data\\.firu\\";
        dir.mkpath( dbPath );
    }
#else    
    dbPath = QDir::homePath() + "/.firu/";
    dir.mkpath( dbPath );
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
            groups.append( QString( "\xE4\xF6" ) );
            groups.append( QString( ",. '-!?" ) );
            groups.append( QString( "abc" ) );
            groups.append( QString( "def" ) );
            groups.append( QString( "ghi" ) );
            groups.append( QString( "jkl" ) );
            groups.append( QString( "mno" ) );
            groups.append( QString( "pqrs" ) );
            groups.append( QString( "tuv" ) );
            groups.append( QString( "wxyz" ) );
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
    FiruApp* self = get();
    if ( !self->m_trainer )
    {
        self->m_trainer = new Trainer( self );
    }
    self->m_trainer->start( dictionary );
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
