#include "word.h"
#include "wordquery.h"
#include "database.h"

// ----------------------------------------------------------------------------

class WordQueryAdapter
{
public:
    static Word::Ptr createFromQuery( const WordsQuery& query, Lang src )
    {
        Word* t = new Word( src );
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
        query->record().text = word.text();
    }
};

// ----------------------------------------------------------------------------

Word::Word( Lang lang )
    : ItemBase( lang )
{
}

// ----------------------------------------------------------------------------

Word::Word( const QString& text, Lang lang )
    : ItemBase( text, lang )
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
    WordByIdQuery::Ptr qry = Database::getQuery<WordByIdQuery>( src );
    if ( qry )
    {
        qry->setPrimaryKey( id );
        if ( qry->start() && qry->next() )
        {
            p = WordQueryAdapter::createFromQuery( *qry, src );
        }
    }
    return p;
}

// ----------------------------------------------------------------------------

Word::List Word::find( const QString& pattern, Lang lang, TextMatch match, int limit )
{
    List words;
    WordsByPatternQuery::Ptr qry = Database::getQuery<WordsByPatternQuery>( lang );
    if ( qry )
    {
        qry->setPattern( pattern, match );
        if ( qry->start() )
        {
            while( qry->next() )
            {
                Ptr w = WordQueryAdapter::createFromQuery( *qry, lang );
                words.append( w );
                if ( limit > 0 && words.count() >= limit )
                {
                    break;
                }
            }
            qDebug() << "Word::find(), found " << words.count() << " results";
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
    WordUpdateQuery::Ptr query = Database::getQuery<WordUpdateQuery>( m_srcLang );
    WordQueryAdapter::setToQuery( query.data(), *this );
    return query->execute();
}

// ----------------------------------------------------------------------------

bool Word::doInsert()
{
    if ( !exists( m_text, getSource() ) )
    {
        WordInsertQuery::Ptr query = Database::getQuery<WordInsertQuery>( m_srcLang );
        WordQueryAdapter::setToQuery( query.data(), *this );
        if ( query->execute() )
        {
            m_id = query->record().id;
            return true;
        }
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
        if ( t->text() == text ) return false;
    }

    Translation::Ptr t( new Translation( QSharedPointer<Word>( this ), text, trg ) );
    list.append( t );
    m_translations[ trg ] = list;
    return true;
}
