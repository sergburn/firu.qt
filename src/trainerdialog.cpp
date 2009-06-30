#include <QKeyEvent>
#include <QDialogButtonBox>
#include <QPushButton>

#include "trainerdialog.h"
#include "ui_trainerdialog.h"
#include "firudebug.h"
#include "firuapp.h"

#define BAR_WIDTH 50
#define BAR_HEIGHT 50

// ----------------------------------------------------------------------------

TrainerDialog::TrainerDialog(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::TrainerDialog),
    m_scene( NULL )
{
    m_ui->setupUi(this);
    m_ui->markGraph->installEventFilter( this );

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

    m_scene = new QGraphicsScene( this );
    m_scene->setSceneRect( 0, 0, BAR_WIDTH, 3 * BAR_HEIGHT );
    m_ui->markGraph->setScene( m_scene );
//    m_ui->markGraph->centerOn( BAR_WIDTH / 2, 3 * BAR_HEIGHT / 2 );
    m_scene->addRect( QRect( -1, -1, BAR_WIDTH + 1, 3 * BAR_HEIGHT + 1 ), QPen( m_clrLevelEmpty ), QBrush( m_clrLevelEmpty ) );
    m_markBar = m_scene->addRect( QRect( 0, 0, BAR_WIDTH, BAR_HEIGHT ), QPen( m_clrLevelEmpty ), QBrush( m_clrLevelEmpty ) );
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

void TrainerDialog::resizeEvent( QResizeEvent* event )
{
    qDebug() << "Old size" << event->oldSize() << "new size" << event->size();
    qDebug() << "Graph size" << m_ui->markGraph->size();
    updateMark();
}

// ----------------------------------------------------------------------------

bool TrainerDialog::eventFilter( QObject *obj, QEvent *event )
{
    if ( event->type() == QEvent::Resize )
    {
        QResizeEvent* sizeEvent = static_cast<QResizeEvent*>(event);
        if ( obj == m_ui->markGraph )
        {
            qDebug() << "Graph old size" << sizeEvent->oldSize() << "new size" << sizeEvent->size();
            updateMark();
        }
    }
    return QDialog::eventFilter( obj, event );
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
    
    QString cmd = keyEvent->text();
    if ( cmd == "*" )
    {
        leftCommand();
    }
    else if ( cmd == "#")
    {
        rightCommand();
    }
    else if ( QRegExp( "[0-9]", Qt::CaseInsensitive ).exactMatch( cmd ) )
    {
        bool ok = false;
        int key = cmd.toInt( &ok );
        if ( ok && key >= 0 && key <= 9 )
        {
            checkNextLetter( m_keyLabels[key]->text() );
        }
    }
    else if ( cmd.length() )
    {
        checkNextLetter( cmd );
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
    connect( m_test.data(), SIGNAL( markChanged() ), SLOT( updateMark() ) );

    m_ui->laQuestion->setText( m_test->question() );
    m_ui->laAnswer->setText("");

    m_keyGroups = FiruApp::getKeypadGroups( m_test->answerLang() );

    m_ui->progressBar->setValue( m_test->numLives() );
    showNextLetters();
    updateMark();
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
    updateMark();
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
    QString hint = m_test->help( m_answerText );
    if ( hint.length() )
        checkAnswer( hint );
}

// ----------------------------------------------------------------------------

void TrainerDialog::showResult()
{
    m_ui->laAnswer->setText( m_test->answer() );
    m_ui->frmKeypad->hide();
    updateMark();
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

void TrainerDialog::updateMark()
{
    if ( m_test )
    {
        drawMark( m_test->currentMark() );
    }
}

// ----------------------------------------------------------------------------

void TrainerDialog::drawMark( Mark::MarkValue mark )
{
    if ( m_scene && m_markBar )
    {
        double h = 0;

        QColor clr;
        switch ( mark )
        {
            case Mark::ToLearn:
                clr = m_clrLevel1;
                h = BAR_HEIGHT;
                break;
            case Mark::WithHints:
                clr = m_clrLevel2;
                h = 2 * BAR_HEIGHT;
                break;
            case Mark::AlmostLearned:
            default:
                clr = m_clrLevel3;
                h = 3 * BAR_HEIGHT;
                break;
        }

        m_markBar->setPen( QPen( clr ) );
        m_markBar->setBrush( QBrush( clr ) );
        m_markBar->setRect( 0, 3 * BAR_HEIGHT - h, BAR_WIDTH, h );

        qDebug() << "View rect" << m_ui->markGraph->size();

//        m_ui->markGraph->fitInView( m_scene->sceneRect() );
    }
}
