#include "reversetest.h"

// ----------------------------------------------------------------------------

ReverseTest::ReverseTest( Translation::Ptr challenge, Word::Ptr answer )
:   m_challenge( challenge ), m_answer( answer ),
    m_result( Incomplete )
{
    m_maxLives = qMin( 3, m_answer->text().length() - 1 );
    m_livesLeft = m_maxLives;
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
    if ( m_challenge->rmark() < Mark::OncePassed )
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
    handleHelpOrMistake();

    if ( m_result == Incomplete )
    {
        QString answer = m_answer->text();
        if ( answer.length() > current.length() &&
             answer.startsWith( current ) )
        {
            return answer.left( current.length() + 1 );
        }
        else
        {
            return current;
        }
    }
    return QString();
}

// ----------------------------------------------------------------------------

void ReverseTest::handleHelpOrMistake()
{
    if ( m_livesLeft > 0 )
    {
        m_livesLeft--;
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
        m_result = ( m_livesLeft < m_maxLives ) ? PassedWithHints : Passed;
    }
    else
    {
        m_result = Failed;
    }

    m_challenge->rmark().updateToTestResult( m_result );
    m_challenge->saveMarks();

    emit finished();
}
