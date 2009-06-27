#include <QKeyEvent>
#include <QDialogButtonBox>
#include <QPushButton>

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
    setKeypadMode();

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

    connect( m_ui->buttonBox, SIGNAL( accepted() ), SIGNAL( onTestDone() ) );

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

void TrainerDialog::keyPressEvent( QKeyEvent *keyEvent )
{
	qDebug("Key %d, scan %d", keyEvent->key(), keyEvent->nativeScanCode());	
    switch( keyEvent->key() )
    {
        case '0':
            setAnswersListMode();
            break;
        case '#':
            setKeypadMode();
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

void TrainerDialog::setAnswersListMode()
{
    m_ui->frmKeypad->setVisible( false );
    m_ui->listAnswers->setVisible( true );
}

// ----------------------------------------------------------------------------

void TrainerDialog::setKeypadMode()
{
    m_ui->frmKeypad->setVisible( true );
    m_ui->listAnswers->setVisible( false );
}

// ----------------------------------------------------------------------------

void TrainerDialog::showTest( ReverseTest::Ptr test )
{
    m_answerText.clear();
    m_keyGroups.clear();

    m_test = test;
    connect( m_test.data(), SIGNAL( finished() ), SLOT( showResult() ) );
    m_ui->laQuestion->setText( m_test->question() );
    m_ui->laAnswer->setText("");

    m_keyGroups = FiruApp::getKeypadGroups( m_test->answerLang() );

    showNextLetters();
    setKeypadMode();

    m_ui->buttonBox->button( QDialogButtonBox::Ok )->setEnabled( false );
    m_ui->buttonBox->button( QDialogButtonBox::Help )->setEnabled( true );

    exec();
}

// ----------------------------------------------------------------------------

void TrainerDialog::checkNextLetter( QString letter )
{
    checkAnswer( m_answerText + letter );
}

// ----------------------------------------------------------------------------

void TrainerDialog::checkAnswer( QString answer )
{
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
        showResult();
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
    m_ui->frmKeypad->show();
}

// ----------------------------------------------------------------------------

void TrainerDialog::showHints()
{
    checkAnswer( m_test->help( m_answerText ) );
}

// ----------------------------------------------------------------------------

void TrainerDialog::showResult()
{
    m_ui->laAnswer->setText( m_test->answer() );
    m_ui->frmKeypad->hide();
    m_ui->buttonBox->button( QDialogButtonBox::Ok )->setEnabled( true );
    m_ui->buttonBox->button( QDialogButtonBox::Ok )->setDefault( true );
    m_ui->buttonBox->button( QDialogButtonBox::Ok )->setFocus();
    m_ui->buttonBox->button( QDialogButtonBox::Help )->setEnabled( false );
}
