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

    static WordByIdQuery::Ptr getSelectQuery( Lang src )
    {
        return getQuery<WordByIdQuery>( src );
    }

    static WordsByPatternQuery::Ptr getSelectByPatternQuery( Lang src )
    {
        return getQuery<WordsByPatternQuery>( src );
    }

    static WordInsertQuery::Ptr getInsertQuery( Lang src )
    {
        return getQuery<WordInsertQuery>( src );
    }

    static WordUpdateQuery::Ptr getUpdateQuery( Lang src )
    {
        return getQuery<WordUpdateQuery>( src );
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
    Query::Ptr query = WordExtension::getUpdateQuery( m_src );
    query->setToQuery( *this );
    return query->execute();
}

// ----------------------------------------------------------------------------

bool Word::doInsert()
{
    Query::Ptr query = WordExtension::getInsertQuery( m_src );
    query->setToQuery( *this );
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
    Translation::List list = m_translations.value( trg );
    if ( list.count() == 0 )
    {
        // try to load translations
        list = Translation::findBySourceEntry( m_id, m_srcLang, trg );
        m_translations[ trg ] = list;
    }
    return list;
}

// ----------------------------------------------------------------------------

bool Translation::addTranslation( const QString& text, Lang trg )
{
    Translation::List list = translations( trg );

    // avoid duplicates
    foreach( Translation t, list )
    {
        if ( t.getText() == text ) return false;
    }

    Translation::Ptr t = new Translation( m_id, text, Langs( m_srcLang, trg ) );
    list.append( t );
    m_translations[ trg ] = list;
    return true;
}
