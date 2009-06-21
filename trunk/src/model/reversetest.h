#ifndef REVERSETEST_H
#define REVERSETEST_H

#include <QObject>
#include <QString>
#include <QStringList>

#include "translation.h"
#include "word.h"

class ReverseTest : public QObject
{
public:
    SHARED_POINTER( ReverseTest );
    ReverseTest( Translation::Ptr challenge, Word::Ptr answer );

    Lang getQuestionLang() const;
    Lang getAnswerLang() const;

    QString getQuestion() const;

    enum AnswerValue { Incorrect, PartiallyCorrect, Correct };
    AnswerValue checkAnswer( const QString& answer );

    /** This hint is not available on level 3, it will return -1. */
    int getAnswerLength() const;

    /** This hint is always available on mobile devices
     *  to allow for easier input */
    QString getNextLetterHint( const QString& current, const QStringList& groups ) const;

    QString getNextLetter( const QString& current ) const;

    /** Using this hint will drop current mark from 3 to 2. */
    QStringList getFullWordHints();

    enum TestResult {
        NotAsked,
        Passed,             // adds 1 to current rate
        PassedWithHints,    // doesn't change rate if 1 or 2, demotes rate 3 to 2
        Failed              // sets current rate 1.
    };

    TestResult getResult() const;

private:
    Translation::Ptr m_challenge;
    Word::Ptr m_answer;
    TestResult m_result;
    QStringList m_hints;
    int m_numMistakes;
};

#endif // REVERSETEST_H
