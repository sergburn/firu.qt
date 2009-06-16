#include "word.h"
#include "itemextensionbase.h"
#include "wordquery.h"
#include "database.h"

// ----------------------------------------------------------------------------

class WordExtension : public ItemExtensionBase
{
public:
    static Word::Ptr createFromQuery( const WordsQuery& query )
    {
        Word* t = new Word( query.source() );
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

    static WordsQuery::Ptr getSelectQuery( Lang src )
    {
        return getQuery<WordsQuery>( src );
    }

    static WordByIdQuery::Ptr getSelectByIdQuery( Lang src )
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

Word::Ptr Word::find( qint64 id, Lang src )
{
    Word::Ptr p;
    WordByIdQuery::Ptr qry = WordExtension::getSelectByIdQuery( src );
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

Word::List Word::find( const QString& pattern, Lang lang, TextMatch match, int limit )
{
    List words;
    WordsByPatternQuery::Ptr qry = WordExtension::getSelectByPatternQuery( lang );
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
    return words;
}

// ----------------------------------------------------------------------------

bool Word::exists( const QString& pattern, Lang lang )
{
    List matches = find( pattern, lang, FullMatch, 1 );
    return matches.count() > 0;
}

// ----------------------------------------------------------------------------

Word::List Word::filter( const List& list, const QString& pattern, TextMatch match )
{
    List newList;
    foreach( Word::Ptr p, list )
    {
        if ( p && p->match( pattern, match ) )
        {
            newList.append( p );
        }
    }
    return newList;
}

// ----------------------------------------------------------------------------

bool Word::doUpdate()
{
    WordUpdateQuery::Ptr query = WordExtension::getUpdateQuery( getSource() );
    WordExtension::setToQuery( query.data(), *this );
    return query->execute();
}

// ----------------------------------------------------------------------------

bool Word::doInsert()
{
    WordInsertQuery::Ptr query = WordExtension::getInsertQuery( getSource() );
    WordExtension::setToQuery( query.data(), *this );
    if ( query->execute() )
    {
        m_id = query->record().id;
        return true;
    }
    return false;
}

// ----------------------------------------------------------------------------

bool Word::doSaveAssociates()
{
    bool ok = true;
    foreach( Lang l, m_translations.keys() )
    {
        foreach( Translation::Ptr t, m_translations[l] )
        {
            ok = t->save();
            if ( !ok ) break;
        }
    }
    return ok;
}

// ----------------------------------------------------------------------------

bool Word::doDelete()
{
    return false;
}

// ----------------------------------------------------------------------------

bool Word::doDeleteAssociates()
{
    // TODO: ??
    return false; //Translation::destroyBySourceEntry( m_id, getSource(), getSource() );
}

// ----------------------------------------------------------------------------

bool Word::match( const QString& pattern, TextMatch match )
{
    switch ( match )
    {
        case FullMatch:
            return m_text == pattern;
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
        list = Translation::findBySourceEntry( m_id, LangPair( m_srcLang, trg ) );
        m_translations[ trg ] = list;
    }
    return list;
}

// ----------------------------------------------------------------------------

bool Word::addTranslation( const QString& text, Lang trg )
{
    Translation::List list = translations( trg );

    // avoid duplicates
    foreach( Translation::Ptr t, list )
    {
        if ( t->getText() == text ) return false;
    }

    Translation::Ptr t( new Translation( QSharedPointer<Word>( this ), text, trg ) );
    list.append( t );
    m_translations[ trg ] = list;
    return true;
}
