#include "trainer.h"
#include "firuapp.h"

// ----------------------------------------------------------------------------

Trainer::Trainer( Dictionary::Ptr dictionary, QObject* parent )
    : QObject( parent ), m_dictionary( dictionary ), m_dialog( NULL )
{
    // connect to Dictionary signals
}

// ----------------------------------------------------------------------------

void Trainer::start()
{
    m_exe = m_dictionary->createReverseExercise();
    // connect to Exercise signals
    showNextTest();
}

// ----------------------------------------------------------------------------

void Trainer::showNextTest()
{
    if ( !m_dialog )
    {
        m_dialog = FiruApp::getTrainerDialog();
        connect( m_dialog.data(), SIGNAL( testFinished() ), SLOT( handleTestDone() ) );
        connect( m_dialog.data(), SIGNAL( testCanceled() ), SLOT( handleTestCancel() ) );
    }

    ReverseTest::Ptr test = m_exe->nextTest();

    if ( test )
    {
        m_dialog->showTest( test );
        m_dialog->show();
        emit onExerciseProgress( m_exe->currentTestIndex(), m_exe->countTests() );
    }
    else
    {
        m_dialog->hide();
        emit onExerciseProgress( 1, 0 );
    }
}

// ----------------------------------------------------------------------------

void Trainer::handleTestDone()
{
    showNextTest();
}

// ----------------------------------------------------------------------------

void Trainer::handleTestCancel()
{
    m_dialog->close();
}
