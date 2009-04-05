#include "firumainwindow.h"

FiruMainWindow::FiruMainWindow(QWidget *parent)
    : QMainWindow(parent)
{
	m_ui.setupUi(this);

	QRect rect = m_appUi.GetClientRect();
	resize( rect.size() );
	
    menuBar()->addAction( m_ui.actionOpenDict );
    menuBar()->addAction( m_ui.actionOpenTrainer );
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
