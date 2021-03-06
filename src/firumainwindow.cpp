#include <QMenu>
#include <QAction>
#include <QKeyEvent>
#include <QSqlTableModel>
#include <QSqlRecord>
#include <QDebug>
#include <QMessageBox>

#include "firuapp.h"
#include "firumainwindow.h"
#include "trainerdialog.h"
#include "entryviewdialog.h"

#include "model/word.h"
#include "model/translation.h"

// ----------------------------------------------------------------------------

FiruMainWindow::FiruMainWindow( QWidget *parent )
    : QMainWindow( parent ), 
    m_reverse( false ), 
    m_loadTimer( this ),
    m_totalWords( 0 )
{
	m_ui.setupUi(this);

#ifndef Q_OS_DARWIN
    menuBar()->addAction( m_ui.actionOpenDict );
    menuBar()->addAction( m_ui.actionOpenTrainer );
//    menuBar()->addAction( m_ui.actionSearch_reverse );
    menuBar()->addAction( m_ui.actionRebuild_Hashes );
    menuBar()->addAction( m_ui.actionResetMarks );
#else
    QMenu* fileMenu = menuBar()->addMenu("File");
    fileMenu->addAction( m_ui.actionOpenDict );
    fileMenu->addAction( m_ui.actionOpenTrainer );
//    menuBar->addAction( m_ui.actionSearch_reverse );
    fileMenu->addAction( m_ui.actionRebuild_Hashes );
    fileMenu->addAction( m_ui.actionResetMarks );
#endif

#ifdef __SYMBIAN32__
    showMaximized();
#else
    QRect rect = m_appUi.GetClientRect();
    setGeometry( rect );
#endif

    m_ui.listSources->installEventFilter( this );
    m_ui.editInput->installEventFilter( this );
    m_ui.progressBar->hide();
    m_ui.laWordCount->hide();
    
    m_loadTimer.setSingleShot( true );
    m_loadTimer.setInterval( 1000 );
    
    connect( 
        m_ui.editInput, SIGNAL( textChanged( const QString& ) ), 
        this, SLOT( onPatternChanged( const QString& ) ) );

    connect( 
        &m_loadTimer, SIGNAL( timeout() ), 
        this, SLOT( searchWords() ) );

    setDirection( QLocale::Finnish, QLocale::Russian );
}

// ----------------------------------------------------------------------------

FiruMainWindow::~FiruMainWindow()
{

}

// ----------------------------------------------------------------------------

bool FiruMainWindow::setDirection( Lang src, Lang trg, bool reverse )
{
    m_dictionary = Dictionary::Ptr( new Dictionary( LangPair( src, trg ) ) );
    if ( m_dictionary->open() )
    {
        m_reverse = reverse;
        updateDirectionLabels();
        m_totalWords = m_dictionary->count( "" );
        m_ui.editInput->clear();
        searchWords();
        return true;
    }
    return false;
}

// ----------------------------------------------------------------------------

void FiruMainWindow::importDict( const QString& file )
{
//    m_ui.prgTask->show();
//    m_ui.prgTask->setValue( 0 );
    Dictionary::import( file );
//    m_ui.prgTask->hide();
}

// ----------------------------------------------------------------------------

void FiruMainWindow::onPatternChanged( const QString& /*pattern*/ )
{
    m_loadTimer.start();
}

// ----------------------------------------------------------------------------

void FiruMainWindow::searchWords()
{
    m_loadTimer.stop();

    QString pattern = m_ui.editInput->text();
    if ( pattern.length() < 1 )
    {
        showWordCount( QString( "There are %1 words in the dictionary.\n"
            "Enter some letters to filter by." ).arg( m_totalWords ) );
        return;
    }

    int count = m_dictionary->count( pattern );
    qDebug() << "Found" << count << "words for pattern " << pattern;

    if ( count < 1 )
    {
        showWordCount( "No matches found." );
    }
    else if ( count > 100 && pattern.length() < 5 )
    {
        showWordCount( QString( "Found %1 matches.\nEnter more letters to filter by.").arg( count ) );
    }
    else
    {
        if ( pattern == m_pattern )
        {
            showList( m_words );
        }
        else
        {
            if ( !pattern.startsWith( m_pattern ) || m_words.count() == 0 )
            {
                m_pattern = pattern; // new pattern
                m_words = m_dictionary->findWords( m_pattern.left( 4 ), StartsWith );
            }
            Word::List words = Word::filter( m_words, pattern, StartsWith );
            showList( words );
        }
    }
}

// ----------------------------------------------------------------------------

void FiruMainWindow::showList( const Word::List& words )
{
    if ( words.count() > 0 )
    {
        m_ui.laWordCount->hide();
        m_ui.listSources->clear();
        foreach( const Word::Ptr wp, words )
        {
            QListWidgetItem* item = new QListWidgetItem( wp->text() );
            item->setData( Qt::UserRole, wp->id() );
            m_ui.listSources->addItem( item );
        }
        m_ui.listSources->show();
    }
    else
    {
        showWordCount( "No matches found." );
    }
}

// ----------------------------------------------------------------------------

void FiruMainWindow::showWordCount( QString text )
{
    m_ui.listSources->hide();
    m_ui.laWordCount->setText( text );
    m_ui.laWordCount->show();
}

// ----------------------------------------------------------------------------

void FiruMainWindow::on_actionOpenTrainer_triggered()
{
    FiruApp::startTrainer( m_dictionary );
}

// ----------------------------------------------------------------------------

void FiruMainWindow::on_actionOpenDict_triggered()
{
    show();
//    QMenu* menu = new QMenu();
//    menu->setTitle( "Menu Title" );
//    menu->setStyleSheet( menuBar()->styleSheet() );
//
//    menu->addAction( m_ui.actionOpenDict );
//    menu->addAction( m_ui.actionOpenTrainer );
//    menu->addSeparator();
//    QAction* cancel = new QAction("Cancel", menu);
//    menu->addAction( cancel );
//    menu->addAction( "close" );
//    menu->setDefaultAction( m_ui.actionOpenTrainer );
//    menu->exec( m_ui.listSources->mapToGlobal( QPoint(0,0)  ) );
//
//    QMenuBar* bar = new QMenuBar();
//    bar->addMenu( menu );
}

// ----------------------------------------------------------------------------

void FiruMainWindow::showTranslation( QListWidgetItem* item )
{
    QString entry = item->text();
    QStringList translations;

    if ( !m_reverse )
    {
        qint64 sid = item->data( Qt::UserRole ).toLongLong();
        Translation::List trans = m_dictionary->findTranslations( sid );
        foreach ( Translation::Ptr tp, trans )
        {
            translations.append( tp->text() );
        }
        m_dictionary->addToUserDict( sid );
    }
    else
    {
//        Word word = m_data.getWord( item->data( Qt::UserRole ).toLongLong() );
//        translations.append( word.getText() );
    }

    EntryViewDialog view;
    view.exec( entry, translations );
}

//void FiruMainWindow::keyPressEvent(QKeyEvent *keyEvent)
//{
//    qDebug("Key %d, scan %d", keyEvent->key(), keyEvent->nativeScanCode()); 
//    switch( keyEvent->key() )
//    {
//        case Qt::Key_Up:
//        case Qt::Key_Down:
//            QApplication::sendEvent( m_ui.listSources, keyEvent );
//            break;
//        default:
//            QApplication::sendEvent( m_ui.editInput, keyEvent );
//            break;
//    }
//}

// ----------------------------------------------------------------------------

bool FiruMainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::KeyPress) 
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
//        switch ( keyEvent->key() )
//        {
//            case Qt::Key_Enter:
//                m_ui.listSources->setFocus();
//                QApplication::sendEvent( m_ui.listSources, event );
//                return true;
//            case Qt::Key_Up:
//                if ( m_ui.listSources->currentRow() == 0 )
//                {
//                    m_ui.editInput->setFocus();
//                    return true;
//                }
//                break;
//            case Qt::Key_Down:
//                if ( obj == m_ui.editInput )
//                {
//                    m_ui.listSources->setFocus();
//                    return true;
//                }
//                break;
//            default:
//                break;
//        }
//        m_ui.editInput->setFocus();
//        QApplication::sendEvent( m_ui.editInput, event );
//        return true;
        if ( obj == m_ui.editInput )
        {
            if ( keyEvent->key() == Qt::Key_Down )
            {
                m_ui.listSources->setFocus();
                return true;
            }
        }
        else if ( obj == m_ui.listSources )
        {
            switch ( keyEvent->key() )
            {
                case Qt::Key_Up:
                    if ( m_ui.listSources->currentRow() == 0 )
                    {
                        m_ui.editInput->setFocus();
                        return true;
                    }
                    break;
                case Qt::Key_Select:
                case Qt::Key_Down:
                    break;
                default:
                    m_ui.editInput->setFocus();
                    QApplication::sendEvent( m_ui.editInput, event );
                    return true;
            }
        }
    }

    // pass the event on to the parent class
    return QMainWindow::eventFilter(obj, event);
}

// ----------------------------------------------------------------------------

//void FiruMainWindow::timerEvent( QTimerEvent* event )
//{
//    setInputWord( m_ui.editInput->text() );
//}

// ----------------------------------------------------------------------------

void FiruMainWindow::on_actionSearch_reverse_toggled( bool reverse )
{
    m_reverse = reverse;
    m_ui.editInput->clear();
    updateDirectionLabels();
    searchWords();
}

// ----------------------------------------------------------------------------

void FiruMainWindow::updateDirectionLabels()
{
    m_ui.laSource->setText( QLocale::languageToString( m_dictionary->source() ) );
    m_ui.laTarget->setText( QLocale::languageToString( m_dictionary->target() ) );
    m_ui.laDir->setText( m_reverse ? "<-" : "->" );
}

// ----------------------------------------------------------------------------

void FiruMainWindow::on_actionRebuild_Hashes_triggered()
{
    QSqlQuery query;
    query.prepare("SELECT count(*) FROM entries_Finnish;");
    query.exec();
    query.next();
    int count = query.value(0).toInt();

    QSqlTableModel table( NULL, Database::instance()->db() );
    table.setTable("entries_Finnish");
    table.select();
    table.setEditStrategy( QSqlTableModel::OnManualSubmit );
    int hashInd = table.record().indexOf("hash");

    m_ui.progressBar->setVisible( true );
    m_ui.progressBar->setMaximum( count  );
    m_ui.progressBar->setMinimum( 0 );
    for ( int i = 0; i < count; i++ )
    {
        QSqlRecord rec = table.record( i );
        QString text = rec.value("text").toString();
        text.chop(1);

        quint64 hash = WordKey::getStringKey( text );
        table.setData( table.index(i,hashInd), hash );

        m_ui.progressBar->setValue( i );
        if ( i % 1000 == 0 )
        {
            table.submitAll();
            qApp->processEvents();
        }
    }
    m_ui.progressBar->setVisible( false );
    table.submitAll();
}

// ----------------------------------------------------------------------------

void FiruMainWindow::on_actionResetMarks_triggered()
{
    QMessageBox::StandardButton button  = QMessageBox::question( this, 
        "Reset all marks", 
        "It will clear your learning set, are you sure?", 
        QMessageBox::Yes | QMessageBox::No );
    
    if ( button == QMessageBox::Yes )
    {
        if ( m_dictionary->clearLearningSet() )
        {
            QMessageBox::information( this, "Reset all marks",
                "All marks are cleared", QMessageBox::Ok );
        }
        else
        {
            QMessageBox::critical( this, "Reset all marks",
                "Operation failed", QMessageBox::Ok );
        }
    }
}
