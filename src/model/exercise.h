#ifndef EXERCISE_H
#define EXERCISE_H

#include <QObject>
#include <QList>

#include "model.h"
#include "reversetest.h"

class Exercise : public QObject
{
    Q_OBJECT
public:
    SHARED_POINTER( Exercise );

    static Ptr generate( LangPair langs );

    int currentTestIndex() const;
    int countTests() const;

    ReverseTest::Ptr nextTest();

signals:
    void onBuildProgress( int progress );

private:
    Exercise();
    Exercise( LangPair langs );
    Q_DISABLE_COPY( Exercise );

    void addTest( ReverseTest::Ptr );
    void selectRandomly( QList<qint64>& ids, Mark::MarkValue level );

private:
    LangPair m_langs;
    int m_index;
    QList<ReverseTest::Ptr> m_tests;

    int m_exeSize;
    float m_shareBetter;
    float m_shareGood;
};

#endif // EXERCISE_H
