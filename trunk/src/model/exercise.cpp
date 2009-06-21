#include <stdlib.h>
#include <QCoreApplication>

#include "exercise.h"
#include "word.h"
#include "translation.h"
#include "database.h"
#include "../firudebug.h"

// ----------------------------------------------------------------------------

Exercise::Exercise( LangPair langs )
    : m_langs( langs ), m_index( 0 ), m_exeSize( 10 ),
    m_shareBetter( 0.3 ), m_shareGood( 0.1 )
{
}

// ----------------------------------------------------------------------------

Exercise::Ptr Exercise::generate( LangPair langs )
{
    Exercise* exe = new Exercise( langs );
    emit exe->onBuildProgress( 0 );
    QCoreApplication::processEvents();

    Translation::List tolearn = Translation::getAllWithRMark( Mark::Good, langs );
    exe->selectRandomly( tolearn, Mark::Good );
    emit exe->onBuildProgress( 30 );
    QCoreApplication::processEvents();

    tolearn = Translation::getAllWithRMark( Mark::Better, langs );
    exe->selectRandomly( tolearn, Mark::Better );
    emit exe->onBuildProgress( 60 );
    QCoreApplication::processEvents();

    tolearn = Translation::getAllWithRMark( Mark::ToLearn, langs );
    exe->selectRandomly( tolearn, Mark::ToLearn );
    emit exe->onBuildProgress( 90 );
    QCoreApplication::processEvents();

//    Word::Ptr word( new Word( "word", QLocale::Finnish ) );
//    Translation::Ptr translation( new Translation( word, "translation", QLocale::Russian ) );
//    ReverseTest::Ptr test( new ReverseTest( translation, word ) );
//
//    exe->addTest( test );
//    emit exe->onBuildProgress( 50 );
//
//    word = Word::Ptr( new Word( "aamu", QLocale::Finnish ) );
//    translation = Translation::Ptr( new Translation( word, "morning", QLocale::Russian ) );
//    test = ReverseTest::Ptr( new ReverseTest( translation, word ) );
//
//    exe->addTest( test );

    emit exe->onBuildProgress( 100 );
    return Exercise::Ptr( exe );
}

// ----------------------------------------------------------------------------

int Exercise::currentTestIndex() const
{
    return m_index;
}

// ----------------------------------------------------------------------------

int Exercise::countTests() const
{
    return m_tests.count();
}

// ----------------------------------------------------------------------------

ReverseTest::Ptr Exercise::nextTest()
{
    ReverseTest::Ptr test;
    if ( m_index < m_tests.count() )
    {
        test = m_tests[ m_index++ ];
    }
    return test;
}

// ----------------------------------------------------------------------------

void Exercise::addTest( ReverseTest::Ptr test )
{
    m_tests.append( test );
}

// ----------------------------------------------------------------------------

void Exercise::selectRandomly( Translation::List& list, Mark::MarkValue level )
{
    int neededAmount = 0;
    switch ( level )
    {
        case Mark::ToLearn:
            neededAmount = m_exeSize - m_tests.count();
            break;
        case Mark::Better:
            neededAmount = m_exeSize * m_shareBetter;
            break;
        case Mark::Good:
            neededAmount = m_exeSize * m_shareGood;
            break;
        default:
            break;
    }

    Translation::List selected;

    if ( neededAmount == 0 )
    {
        return;
    }
    else if ( neededAmount >= list.count() )
    {
        selected = list;
    }
    else
    {
        QList<int> selection;
        while ( neededAmount < list.count() )
        {
            int index = qrand() / ((double) RAND_MAX) * list.count();
            if ( selection.indexOf( index ) < 0 )
            {
                selection.append( index );
                neededAmount++;
            }
        }

        foreach( int i, selection )
        {
            selected.append( list[i] );
        }
    }

    foreach( Translation::Ptr t, selected )
    {
        Word::Ptr word = t->getWord();
        ReverseTest::Ptr test( new ReverseTest( t, word ) );
        addTest( test );
        qDebug() << "Added test:" << t->fmark() << t->rmark() << "'" << word->text() << "' -> '" << t->text() << "'";
    }
}
