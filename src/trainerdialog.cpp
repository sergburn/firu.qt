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
    m_ui(new Ui::TrainerDialog),
    m_scene( NULL )
{
    m_ui->setupUi(this);

    m_keyLabels.append( m_ui->la0 );
    m_keyLabels.append( m_ui->la1 );
    m_keyLabels.append( m_ui->la2 );
    m_keyLabels.append( m_ui->la3 );
    m_keyLabels.append( m_ui->la4 );
    m_keyLabels.append( m_ui->la5 );
    m_keyLabels.append( m_ui->la6 );
    m_keyLabels.append( m_ui->la7 );
    m_keyLabels.append( m_ui->la8 );
    m_keyLabels.append( m_ui->la9 );

#ifdef __SYMBIAN32__
    showMaximized();
#else
    QRect rect = m_appUi.GetClientRect();
    setGeometry( rect );
#endif

    m_clrLevelEmpty = Qt::white;
    m_clrLevel1 = Qt::red;
    m_clrLevel2 = QColor(255,170,0);
    m_clrLevel3 = Qt::green;

    int w = m_ui->markGraph->sizeHint().width() / 3;
    int h = m_ui->markGraph->sizeHint().height();

    m_scene = new QGraphicsScene( this );
    m_ui->markGraph->setScene( m_scene );
    m_itemLevel1 = m_scene->addRect( QRect( 0, 0, w, h ), QPen( m_clrLevel1 ), QBrush( m_clrLevelEmpty ) );
    m_itemLevel2 = m_scene->addRect( QRect( w+1, 0, w, h ), QPen( m_clrLevel2 ), QBrush( m_clrLevelEmpty ) );
    m_itemLevel3 = m_scene->addRect( QRect( 2*(w+1), 0, w, h ), QPen( m_clrLevel3 ), QBrush( m_clrLevelEmpty ) );
    m_ui->markGraph->centerOn( m_itemLevel2 );
    m_ui->markGraph->fitInView( m_scene->sceneRect() );
}

// ----------------------------------------------------------------------------

TrainerDialog::~TrainerDialog()
{
    delete m_ui;
    delete m_scene;
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

void TrainerDialog::resizeEvent( QResizeEvent * event )
{
    if ( m_scene )
    {
        m_ui->markGraph->fitInView( m_scene->sceneRect() );
    }
}

// ----------------------------------------------------------------------------

void TrainerDialog::keyPressEvent( QKeyEvent *keyEvent )
{
    qDebug() << "key" << keyEvent->key()
            << "scan" << keyEvent->nativeScanCode()
            << "text" << keyEvent->text();

    if ( m_test->testResult() != Incomplete )
    {
        emit testFinished();
        return;
    }

    switch( keyEvent->key() )
    {
        case '0':
            leftCommand();
            break;
        case '#':
            rightCommand();
            break;
        default:
            if ( keyEvent->key() >= '1' && keyEvent->key() <= '9' )
            {
                int index = keyEvent->key() - '1';
                checkNextLetter( m_keyLabels[index]->text() );
            }
            else
            {
                checkNextLetter( keyEvent->text() );
            }
            break;
    }
}

// ----------------------------------------------------------------------------

void TrainerDialog::mousePressEvent( QMouseEvent *mouseEvent )
{
    QPoint pos = mouseEvent->pos();
    qDebug() << "Button" << mouseEvent->button() << ", pos" << pos;

    if ( m_test->testResult() != Incomplete )
    {
        emit testFinished();
        return;
    }

    QRect r = m_ui->laRightCmd->rect();
    QRect rect( m_ui->laRightCmd->mapTo( this, r.topLeft() ), m_ui->laRightCmd->mapTo( this, r.bottomRight() ) );
    if ( rect.contains( pos ) )
    {
        rightCommand();
        return;
    }

    r = m_ui->laLeftCmd->rect();
    rect = QRect( m_ui->laLeftCmd->mapTo( this, r.topLeft() ), m_ui->laLeftCmd->mapTo( this, r.bottomRight() ) );
    if ( rect.contains( pos ) )
    {
        leftCommand();
        return;
    }

    foreach ( QLabel* la, m_keyLabels )
    {
        QRect r = la->rect();
        QRect rect( la->mapTo( this, r.topLeft() ), la->mapTo( this, r.bottomRight() ) );
        if ( rect.contains( pos ) )
        {
            checkNextLetter( la->text() );
            return;
        }
    }
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

    m_ui->progressBar->setValue( m_test->numLives() );
    showNextLetters();
    showMark( m_test->currentMark() );
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
        m_ui->progressBar->setValue( m_test->numLives() );
        showNextLetters();
    }
    else if ( av == ReverseTest::Correct )
    {
        m_answerText = answer;
        showResult();
    }
    m_ui->progressBar->setValue( m_test->numLives() );
    showMark( m_test->currentMark() );
}

// ----------------------------------------------------------------------------

void TrainerDialog::showNextLetters()
{
    QString letters = m_test->getNextLetterHint( m_answerText, m_keyGroups );
    for ( int i = 0; i < letters.length(); i++ )
    {
        m_keyLabels[ i ]->setText( QString( letters[i] ) );
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
    showMark( m_test->currentMark() );
}

// ----------------------------------------------------------------------------

void TrainerDialog::leftCommand()
{
    emit testCanceled();
}

// ----------------------------------------------------------------------------

void TrainerDialog::rightCommand()
{
    if ( m_test->testResult() == Incomplete )
    {
        showHints();
    }
}

// ----------------------------------------------------------------------------

void TrainerDialog::showMark( Mark::MarkValue mark )
{
    m_itemLevel1->setBrush( ( mark >= Mark::ToLearn ) ? QBrush( m_clrLevel1 ) : QBrush( m_clrLevelEmpty ) );
    m_itemLevel2->setBrush( ( mark >= Mark::OncePassed ) ? QBrush( m_clrLevel2 ) : QBrush( m_clrLevelEmpty ) );
    m_itemLevel3->setBrush( ( mark >= Mark::AlmostLearned ) ? QBrush( m_clrLevel3 ) : QBrush( m_clrLevelEmpty ) );
    m_ui->markGraph->fitInView( m_scene->sceneRect() );
    qDebug() << "Scene rect" << m_scene->sceneRect();
    qDebug() << "View rect" << m_ui->markGraph->size();
}
