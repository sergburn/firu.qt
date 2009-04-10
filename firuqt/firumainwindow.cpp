#include "firumainwindow.h"
#include "trainerdialog.h"
#include <QMenu>
#include <QAction>

FiruMainWindow::FiruMainWindow(QWidget *parent)
    : QMainWindow(parent)
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
    menu->exec( m_ui.listSources->mapToGlobal( QPoint(0,0)  ) );
//
//    QMenuBar* bar = new QMenuBar();
//    bar->addMenu( menu );
}
