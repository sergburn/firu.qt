#include "reversetest.h"

const int MAX_LIVES = 3;

// ----------------------------------------------------------------------------

ReverseTest::ReverseTest( Translation::Ptr challenge, Word::Ptr answer )
:   m_challenge( challenge ), m_answer( answer ),
    m_livesLeft( MAX_LIVES ),
    m_result( Incomplete )
{
}

// ----------------------------------------------------------------------------

QString ReverseTest::question() const
{
    return m_challenge->text();
}

// ----------------------------------------------------------------------------

QString ReverseTest::answer() const
{
    if ( m_result > Incomplete )
    {
        return m_answer->text();
    }
    return QString();
}

// ----------------------------------------------------------------------------

Lang ReverseTest::questionLang() const
{
    return m_challenge->getTarget();
}

// ----------------------------------------------------------------------------

Lang ReverseTest::answerLang() const
{
    return m_answer->getSource();
}

// ----------------------------------------------------------------------------

int ReverseTest::answerLength() const
{
    if ( m_challenge->rmark() < Mark::AlmostLearned )
    {
        return m_answer->text().length();
    }
    else
    {
        return -1;
    }
}

// ----------------------------------------------------------------------------

ReverseTest::AnswerValue ReverseTest::checkAnswer( const QString& answer )
{
    if ( m_result == Incomplete )
    {
        if ( answer == m_answer->text() )
        {
            setTestPassed( true );
            return Correct;
        }
        else if ( m_answer->text().startsWith( answer ) )
        {
            return PartiallyCorrect;
        }
        else
        {
            handleHelpOrMistake();
            return Incorrect;
        }
    }
    else
    {
        // better raise exception
        return Incorrect;
    }
}

// ----------------------------------------------------------------------------

QString ReverseTest::getNextLetterHint( const QString& current, const QStringList& groups ) const
{
    QString hints;
    QString answer = m_answer->text();
    if ( answer.length() > current.length() &&
         answer.startsWith( current ) )
    {
        QChar next = answer[ current.length() ];
        foreach( QString g, groups )
        {
            if ( g.contains( next ) )
            {
                hints += next;
            }
            else
            {
                hints += g[ qrand() % g.length() ];
            }
        }
    }
    return hints;
}

// ----------------------------------------------------------------------------

QString ReverseTest::help( const QString& current )
{
    QString empty;
    if ( m_result != Incomplete ) return empty;

    QString answer = m_answer->text();
    if ( answer.length() > current.length() + 1 ) // help never tells last letter
    {
        handleHelpOrMistake();
        if ( answer.startsWith( current ) )
        {
            return answer.left( current.length() + 1 );
        }
    }
    return empty;
}

// ----------------------------------------------------------------------------

void ReverseTest::handleHelpOrMistake()
{
    if ( m_livesLeft > 0 )
    {
        m_livesLeft--;
        if ( currentMark() == Mark::AlmostLearned )
        {
            m_challenge->rmark().updateToTestResult( PassedWithHints );
            m_challenge->saveMarks();
            emit markChanged();
        }
    }
    else
    {
        setTestPassed( false );
    }
}

// ----------------------------------------------------------------------------

void ReverseTest::setTestPassed( bool passed )
{
    if ( m_result != Incomplete ) return; // better - assert or exception

    if ( passed )
    {
        m_result = ( m_livesLeft < MAX_LIVES ) ? PassedWithHints : Passed;
    }
    else
    {
        m_result = Failed;
    }

    m_challenge->rmark().updateToTestResult( m_result );
    m_challenge->saveMarks();

    Translation::Ptr t = Translation::find( 
            m_challenge->id(), m_challenge->getLangs() );
    qDebug() << "TR" << t->id() 
        << "fmark" << t->fmark() 
        << "rmark" << t->rmark();
    
    emit finished();
}
