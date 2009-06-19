#include "reversetest.h"

// ----------------------------------------------------------------------------

ReverseTest::ReverseTest( Translation::Ptr challenge, Word::Ptr answer )
:   m_challenge( challenge ), m_answer( answer ),
    m_result(NotAsked), m_numMistakes( 0 )
{
}

// ----------------------------------------------------------------------------

int ReverseTest::getAnswerLength() const
{
    if ( m_challenge->rmark() < Mark::Good )
    {
        return m_answer->getText().length();
    }
    else
    {
        return -1;
    }
}

// ----------------------------------------------------------------------------

QStringList ReverseTest::getFullWordHints()
{
    if ( m_challenge->rmark() >= Mark::Good )
    {
        m_challenge->rmark().downgrade();
    }
    return m_hints;
}

// ----------------------------------------------------------------------------

QString ReverseTest::getNextLetterHint( const QString& current, const QStringList& groups ) const
{
    QString hints;
    QString answer = m_answer->getText();
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

QString ReverseTest::getQuestion() const
{
    return m_challenge->getText();
}

// ----------------------------------------------------------------------------

Lang ReverseTest::getQuestionLang() const
{
    return m_challenge->getTarget();
}

// ----------------------------------------------------------------------------

Lang ReverseTest::getAnswerLang() const
{
    return m_answer->getSource();
}

// ----------------------------------------------------------------------------

ReverseTest::AnswerValue ReverseTest::checkAnswer( const QString& answer )
{
    if ( answer == m_answer->getText() )
    {
        return Correct;
    }
    else if ( m_answer->getText().contains( answer ) )
    {
        return PartiallyCorrect;
    }
    else
    {
        m_numMistakes++;
        return Incorrect;
    }
}
