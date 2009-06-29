#ifndef REVERSETEST_H
#define REVERSETEST_H

#include <QObject>
#include <QString>
#include <QStringList>

#include "translation.h"
#include "word.h"

class ReverseTest : public QObject
{
    Q_OBJECT
public:
    SHARED_POINTER( ReverseTest );
    ReverseTest( Translation::Ptr challenge, Word::Ptr answer );

    Lang questionLang() const;
    Lang answerLang() const;

    QString question() const;

    /** Returs answer string, but only for finished test */
    QString answer() const;
    /** This hint is not available on level 3, it will return -1. */
    int answerLength() const;

    enum AnswerValue { Incorrect, PartiallyCorrect, Correct };
    AnswerValue checkAnswer( const QString& answer );

    /** This hint is always available on mobile devices
     *  to allow for easier input */
    QString getNextLetterHint( const QString& current, const QStringList& groups ) const;

    /** Adds one letter */
    QString help( const QString& current );

    /** @return Number of lives left */
    int numLives() const { return m_livesLeft; }

    TestResult testResult() const { return m_result; }
    Mark::MarkValue currentMark() const { return m_challenge->rmark()(); }

signals:
    void finished();
    void markChanged();

private:
    void handleHelpOrMistake();
    void setTestPassed( bool passed );

private:
    Translation::Ptr m_challenge;
    Word::Ptr m_answer;

    int m_livesLeft;

    TestResult m_result;
};

#endif // REVERSETEST_H
