#include "firumainwindow.h"
#include "trainerdialog.h"
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
    m_data.importDictionary( file );
}

void FiruMainWindow::updateList()
{
    QList<Word> words = m_data.searchWords( m_word );
    m_ui.listSources->clear();
    m_listItems.clear();
    foreach( Word w, words )
    {
//        QListWidgetItem item( w.getText() );
//        item.setData( Qt::UserRole, w.getId() );
//        m_listItems.append( item );
//        m_ui.listSources->addItem( m_listItems.back() );
        m_ui.listSources->addItem( w.getText() );
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
