#include "word.h"
#include "itemextensionbase.h"
#include "wordquery.h"
#include "database.h"

// ----------------------------------------------------------------------------

class WordExtension : public ItemExtensionBase
{
public:
    static Word::Ptr createFromQuery( const WordsQuery* query )
    {
        Word* t = new Word( query->source() );
        if ( t )
        {
            t->m_id = query.record().id;
            t->m_text = query.record().text;
            t->m_changed = 0;
        }
        return Word::Ptr( t );
    }

    static void setToQuery( WordsQuery* query, const Word& word )
    {
        query->record().id = word.getId();
        query->record().text = word.getText();
    }

    static WordSelectQuery::Ptr getSelectQuery( Lang src )
    {
        return getQuery<WordSelectByIdQuery>( src );
    }

    static WordsByPatternQuery::Ptr getSelectByPatternQuery( Lang src )
    {
        return getQuery<WordsByPatternQuery>( src );
    }

    static WordInsertQuery::Ptr getInsertQuery( Lang src, const Word& word )
    {
        WordInsertQuery::Ptr q = getQuery<WordInsertQuery>( src );
        setToQuery( q, word );
        return q;
    }

    static WordUpdateQuery::Ptr getUpdateQuery( Lang src, const Word& word )
    {
        WordUpdateQuery::Ptr q = getQuery<WordUpdateQuery>( src );
        setToQuery( q, word );
        return q;
    }
};

// ----------------------------------------------------------------------------

Word::Word( Lang lang )
    : ItemBase( lang )
{
    m_extension = new WordExtension();
}

// ----------------------------------------------------------------------------

Word::Word( const QString& text, Lang lang )
    : ItemBase( lang ), m_text( text )
{
}

// ----------------------------------------------------------------------------

void Word::setText( const QString& text )
{
    m_text = text;
    m_changed = true;
}

// ----------------------------------------------------------------------------

Word::Ptr Word::create( const QString& text, Lang lang )
{
    Word* w = new Word( text, lang );
    save();
    return Ptr( w );
}

// ----------------------------------------------------------------------------

Word::Ptr Word::find( qint64 id )
{
    Word::Ptr p;
    WordsQuery::Ptr qry = WordExtension::getSelectQuery( m_srcLang );
    if ( qry )
    {
        qry->setPrimaryKey( id );
        if ( qry->start() && qry->next() )
        {
            p = WordExtension::createFromQuery( *qry );
        }
    }
    return p;
}

// ----------------------------------------------------------------------------

Word::List Word::find( const QString& pattern, TextMatch match, int limit )
{
    List words;
    WordQuery::Ptr qry = WordExtension::getSelectByPatternQuery( m_srcLang );
    if ( qry )
    {
        qry->setPattern( pattern, match );
        if ( qry->start() )
        {
            while( qry->next() )
            {
                Ptr w = WordExtension::createFromQuery( *qry );
                words.append( w );
                if ( limit > 0 && words.count() >= limit )
                {
                    break;
                }
            }
        }
    }
    return p;
}

// ----------------------------------------------------------------------------

bool Word::exists( const QString& pattern )
{
    List matches = find( pattern, FullMatch, 1 );
    return matches.count() > 0;
}

// ----------------------------------------------------------------------------

Word::List Word::filter( const List& list, const QString& pattern, TextMatch match )
{
    List list;
    foreach( Word::Ptr p, list )
    {
        if ( p && p->match( pattern, match ) )
        {
            list.append( p );
        }
    }
    return list;
}

// ----------------------------------------------------------------------------

bool Word::doUpdate()
{
    WordUpdateQuery::Ptr query = WordExtension::getUpdateQuery( m_src, *this );
    return query->execute();
}

// ----------------------------------------------------------------------------

bool Word::doInsert()
{
    WordInsertQuery::Ptr query = WordExtension::getInsertQuery( m_src, *this );
    ok = query->execute();
    if ( ok )
    {
        m_id = query->record().id;
    }
}

// ----------------------------------------------------------------------------

bool Word::doSaveAssociates()
{
    bool ok = true;
    foreach( Translation::Ptr t, m_translations )
    {
        ok = t->save( m_id );
        if ( !ok ) break;
    }
    return ok;
}

// ----------------------------------------------------------------------------

bool Word::doDeleteAssociates()
{
    return Translation::destroyBySourceEntry( m_id, m_srcLang, m_trgLang );
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
        m_trgLang = trg;
        m_translations = Translation::findBySourceEntry( m_id, m_srcLang, m_trgLang );
        // drop marks for these translations
        foreach( Translation t, m_translations )
        {
            t.fmark().restart();
            t.rmark().restart();
        }
        Translation::saveMarks( m_translations, m_id );
    }
    return m_translations;
}
