#include <QKeyEvent>

#include "trainerdialog.h"
#include "ui_trainerdialog.h"
#include "firudebug.h"
#include "firuapp.h"

// ----------------------------------------------------------------------------

TrainerDialog::TrainerDialog(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::TrainerDialog)
{
    m_ui->setupUi(this);
    showKeypad();

    m_keyLabels.append( m_ui->la1 );
    m_keyLabels.append( m_ui->la2 );
    m_keyLabels.append( m_ui->la3 );
    m_keyLabels.append( m_ui->la4 );
    m_keyLabels.append( m_ui->la5 );
    m_keyLabels.append( m_ui->la6 );
    m_keyLabels.append( m_ui->la7 );
    m_keyLabels.append( m_ui->la8 );
    m_keyLabels.append( m_ui->la9 );
    m_keyLabels.append( m_ui->la10 );
    m_keyLabels.append( m_ui->la11 );
    m_keyLabels.append( m_ui->la12 );

#ifdef __SYMBIAN32__
    showMaximized();
#else
    QRect rect = m_appUi.GetClientRect();
    setGeometry( rect );
#endif
}

// ----------------------------------------------------------------------------

TrainerDialog::~TrainerDialog()
{
    delete m_ui;
}

// ----------------------------------------------------------------------------

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

// ----------------------------------------------------------------------------

void TrainerDialog::keyPressEvent(QKeyEvent *keyEvent)
{
	qDebug("Key %d, scan %d", keyEvent->key(), keyEvent->nativeScanCode());	
    switch( keyEvent->key() )
    {
        case '0':
            showAnswersList();
            break;
        case '#':
            showKeypad();
            break;
        default:
            checkNextLetter( keyEvent->text() );
            break;
    }
}

// ----------------------------------------------------------------------------

void TrainerDialog::mousePressEvent( QMouseEvent *mouseEvent )
{
    QPoint pos = mouseEvent->pos();
    qDebug() << "Button" << mouseEvent->button() << ", pos" << pos;
    foreach ( QLabel* la, m_keyLabels )
    {
        QRect r = la->rect();
        QRect rect( la->mapTo( this, r.topLeft() ), la->mapTo( this, r.bottomRight() ) );
        if ( rect.contains( pos ) )
        {
            checkNextLetter( la->text() );
        }
    }
}

// ----------------------------------------------------------------------------

void TrainerDialog::showAnswersList()
{
    m_ui->frmKeypad->setVisible( false );
    m_ui->listAnswers->setVisible( true );
}

// ----------------------------------------------------------------------------

void TrainerDialog::showKeypad()
{
    m_ui->frmKeypad->setVisible( true );
    m_ui->listAnswers->setVisible( false );
}

// ----------------------------------------------------------------------------

void TrainerDialog::showTest( ReverseTest::Ptr test )
{
    m_test = test;
    m_ui->laQuestion->setText( m_test->getQuestion() );
    m_ui->laAnswer->setText("");

    m_keyGroups = FiruApp::getKeypadGroups( m_test->getAnswerLang() );

    showNextLetters();
}

// ----------------------------------------------------------------------------

void TrainerDialog::checkNextLetter( QString letter )
{
    QString answer = m_answerText + letter;
    ReverseTest::AnswerValue av = m_test->checkAnswer( answer );
    if ( av == ReverseTest::PartiallyCorrect )
    {
        m_answerText = answer;
        m_ui->laAnswer->setText( m_answerText );
        showNextLetters();
    }
    else if ( av == ReverseTest::Correct )
    {
        m_answerText = answer;
        m_ui->laAnswer->setText( m_answerText );
        hideNextLetters();
        // emit done();
    }
}

// ----------------------------------------------------------------------------

void TrainerDialog::showNextLetters()
{
    QString letters = m_test->getNextLetterHint( m_answerText, m_keyGroups );
    for ( int i = 0; i < letters.length(); i++ )
    {
        m_keyLabels[ i+1 ]->setText( QString( letters[i] ) );
    }
}

// ----------------------------------------------------------------------------

void TrainerDialog::hideNextLetters()
{
    foreach(QLabel* l, m_keyLabels)
    {
        l->hide();
    }
}
