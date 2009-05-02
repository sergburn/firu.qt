#include "firumainwindow.h"
#include "trainerdialog.h"
#include "entryviewdialog.h"
#include <QMenu>
#include <QAction>
#include <QKeyEvent>

FiruMainWindow::FiruMainWindow( Data& data, QWidget *parent)
    : QMainWindow( parent ), m_data( data )
{
	m_ui.setupUi(this);

    menuBar()->addAction( m_ui.actionOpenDict );
    menuBar()->addAction( m_ui.actionOpenTrainer );

#ifdef __SYMBIAN32__
    showMaximized();
#else
    QRect rect = m_appUi.GetClientRect();
    setGeometry( rect );
#endif

    m_ui.listSources->installEventFilter( this );
    m_ui.editInput->installEventFilter( this );
    m_ui.prgTask->hide();
    
    m_data.select( QLocale::Finnish, QLocale::Russian );
    connect( &m_data, SIGNAL( progress( int ) ), m_ui.prgTask, SLOT( setValue( int ) ) );
//    connect( m_data, SIGNAL( searchComplete ), this, SLOT( onSearchComplete ) );
}

FiruMainWindow::~FiruMainWindow()
{

}

void FiruMainWindow::setInputWord( QString word )
{
    if ( word != m_word )
    {
        m_word = word;
        emit inputWordChanged( m_word );
        
        updateList();
    }
}

void FiruMainWindow::importDict( const QString& file )
{
    m_ui.prgTask->show();
    m_ui.prgTask->setValue( 0 );
    m_data.importDictionary( file );
    m_ui.prgTask->hide();
}

void FiruMainWindow::updateList()
{
    m_ui.listSources->clear();
    m_listItems.clear();
    if ( m_word.length() > 2 )
    {
        QList<Word> words = m_data.searchWords( m_word );
        foreach( Word w, words )
        {
//            QListWidgetItem item( w.getText() );
//            item.setData( Qt::UserRole, w.getId() );
//            m_listItems.append( item );
//            m_ui.listSources->addItem( m_listItems.back() );
            m_ui.listSources->addItem( w.getText() );
        }
    }
}

void FiruMainWindow::on_actionOpenTrainer_triggered()
{
    TrainerDialog t(this);
    t.exec();
}

void FiruMainWindow::on_actionOpenDict_triggered()
{
    QMenu* menu = new QMenu();
    menu->setTitle( "Menu Title" );
    menu->setStyleSheet( menuBar()->styleSheet() );

    menu->addAction( m_ui.actionOpenDict );
    menu->addAction( m_ui.actionOpenTrainer );
    menu->addSeparator();
    QAction* cancel = new QAction("Cancel", menu);
    menu->addAction( cancel );
    menu->addAction( "close" );
    menu->setDefaultAction( m_ui.actionOpenTrainer );
    menu->exec( m_ui.listSources->mapToGlobal( QPoint(0,0)  ) );
//
//    QMenuBar* bar = new QMenuBar();
//    bar->addMenu( menu );
}

void FiruMainWindow::showTranslation( QListWidgetItem* item )
{
    Word word( item->data( Qt::UserRole ).toLongLong(), item->text() );
    QList<Translation> trans = m_data.getTranslations( word );

    EntryViewDialog view;
    view.exec( word, trans );
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

bool FiruMainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::KeyPress) 
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
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
