#include "exercise.h"
#include "word.h"
#include "translation.h"
#include "database.h"

// ----------------------------------------------------------------------------

Exercise::Exercise( LangPair langs )
    : m_langs( langs ), m_index( 0 )
{
}

// ----------------------------------------------------------------------------

Exercise::Ptr Exercise::generate( LangPair langs )
{
    Exercise* exe = new Exercise( langs );

    Word::Ptr word( new Word( "word", QLocale::Finnish ) );
    Translation::Ptr translation( new Translation( word, "translation", QLocale::Russian ) );
    ReverseTest::Ptr test( new ReverseTest( translation, word ) );

    exe->addTest( test );
    emit exe->onBuildProgress( 50 );

    word = Word::Ptr( new Word( "aamu", QLocale::Finnish ) );
    translation = Translation::Ptr( new Translation( word, "morning", QLocale::Russian ) );
    test = ReverseTest::Ptr( new ReverseTest( translation, word ) );

    exe->addTest( test );
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
