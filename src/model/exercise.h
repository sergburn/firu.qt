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

private:
    LangPair m_langs;
    int m_index;
    QList<ReverseTest::Ptr> m_tests;
};

#endif // EXERCISE_H
