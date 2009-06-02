#include "word.h"
#include "wordquery.h"

// ----------------------------------------------------------------------------

class WordExtension
{
public:
    static Word::Ptr createFromQuery( const WordQuery& query )
    {
        Word* t = new Word( query.source, query.target );
        if ( t )
        {
            t->m_id = query.record().id;
            t->m_text = query.record().text;
        }
        return Word::Ptr( t );
    }

    WordQuery::Ptr query( Lang src, Lang trg )
    {
        if ( !m_query )
        {
            m_query = DbSchema::getWordQuery( src, trg );
        }
        return m_query;
    }

private:
    WordQuery::Ptr m_query;
};

// ----------------------------------------------------------------------------

Word::Ptr Word::find( qint64 )
{
    Word::Ptr p;
    WordQuery::Ptr qry = m_extension->query( src, trg );
    if ( qry )
    {
        qry->filterId = id;
        qry->start();
        if ( qry->next() )
        {
            p = WordUtility::createFromQuery( *qry );
        }
        delete qry;
    }
    return p;
}

// ----------------------------------------------------------------------------

Word::List Word::find( const QString& pattern, TextMatch match )
{
}

// ----------------------------------------------------------------------------

Word::List Word::filter( const List& list, const QString& pattern, TextMatch match )
{
    List list;
    foreach( Word::Ptr p, list )
    {
        if ( p )
        {
            Word& w = *p;
            if ( w.match( pattern, match ) )
            {
                list.append( p );
            }
        }
    }
    return list;
}

// ----------------------------------------------------------------------------

bool Word::match( const QString& pattern, TextMatch match )
{
    switch ( match )
    {
        case FullMatch:
            return m_text == text;
        case StartsWith:
            return m_text.startsWith( pattern );
        case Contains:
            return m_text.contains( pattern );
        case EndsWith:
            return m_text.endsWith( pattern );
        default:
            return false;
    }
}

// ----------------------------------------------------------------------------

Translation::List Word::translations( Lang trg )
{
    if ( m_translations.count() == 0 )
    {
        // try to load translations
        m_translations = Translation::findBySourceEntry( m_id, m_lang, trg );
    }
    return m_translations;
}
