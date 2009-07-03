#include <QMenu>
#include <QAction>
#include <QKeyEvent>

#include "firuapp.h"
#include "firumainwindow.h"
#include "trainerdialog.h"
#include "entryviewdialog.h"

#include "model/word.h"
#include "model/translation.h"

// ----------------------------------------------------------------------------

FiruMainWindow::FiruMainWindow( QWidget *parent)
    : QMainWindow( parent ), m_reverse( false )
{
	m_ui.setupUi(this);

    menuBar()->addAction( m_ui.actionOpenDict );
    menuBar()->addAction( m_ui.actionOpenTrainer );
    menuBar()->addAction( m_ui.actionSearch_reverse );

#ifdef __SYMBIAN32__
    showMaximized();
#else
    QRect rect = m_appUi.GetClientRect();
    setGeometry( rect );
#endif

    m_ui.listSources->installEventFilter( this );
    m_ui.editInput->installEventFilter( this );
    m_ui.prgTask->hide();
    
    setDirection( QLocale::Finnish, QLocale::Russian );
//    connect( &m_data, SIGNAL( progress( int ) ), m_ui.prgTask, SLOT( setValue( int ) ) );
//    connect( m_data, SIGNAL( searchComplete ), this, SLOT( onSearchComplete ) );
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
        updateList();
        return true;
    }
    return false;
}

// ----------------------------------------------------------------------------

void FiruMainWindow::setInputWord( QString word )
{
    if ( word != m_pattern )
    {
        m_pattern = word;
        emit inputWordChanged( m_pattern );
        
        updateList();
    }
}

// ----------------------------------------------------------------------------

void FiruMainWindow::importDict( const QString& file )
{
    m_ui.prgTask->show();
    m_ui.prgTask->setValue( 0 );
    Dictionary::import( file );
    m_ui.prgTask->hide();
}

// ----------------------------------------------------------------------------

void FiruMainWindow::updateList()
{
    m_ui.listSources->clear();
    if ( m_pattern.length() > 2 )
    {
        Word::List words;
        if ( !m_reverse )
        {
            words = m_dictionary->findWords( m_pattern, StartsWith );
        }
        else
        {
//            words = Translation::find( m_pattern, Contains );
        }
        foreach( Word::Ptr wp, words )
        {
            QListWidgetItem* item = new QListWidgetItem( wp->text() );
            item->setData( Qt::UserRole, wp->id() );
            m_ui.listSources->addItem( item );
        }
    }
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
            if ( keyEvent->key() == Qt::Key_Up )
            {
                if ( m_ui.listSources->currentRow() == 0 )
                {
                    m_ui.editInput->setFocus();
                    return true;
                }
            }
            else if ( keyEvent->key() != Qt::Key_Down )
            {
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

void FiruMainWindow::on_actionSearch_reverse_toggled( bool reverse )
{
    m_reverse = reverse;
    m_ui.editInput->clear();
    updateDirectionLabels();
    updateList();
}

// ----------------------------------------------------------------------------

void FiruMainWindow::updateDirectionLabels()
{
    m_ui.laSource->setText( QLocale::languageToString( m_dictionary->source() ) );
    m_ui.laTarget->setText( QLocale::languageToString( m_dictionary->target() ) );
    m_ui.laDir->setText( m_reverse ? "<-" : "->" );
}
