#include "trainerdialog.h"
#include "ui_trainerdialog.h"

#include <QKeyEvent>

TrainerDialog::TrainerDialog(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::TrainerDialog)
{
    m_ui->setupUi(this);
    m_ui->listAnswers->setVisible( false );
}

TrainerDialog::~TrainerDialog()
{
    delete m_ui;
}

void TrainerDialog::changeEvent(QEvent *e)
{
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void TrainerDialog::keyPressEvent(QKeyEvent *keyEvent)
{
	qDebug("Key %d, scan %d", keyEvent->key(), keyEvent->nativeScanCode());	
    switch( keyEvent->key() )
    {
        case '0':
            showAnswersList();
            break;
    }
}

void TrainerDialog::showAnswersList()
{
    m_ui->frmKeypad->setVisible( false );
    m_ui->listAnswers->setVisible( true );
}
