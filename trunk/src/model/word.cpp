#include "word.h"
#include "wordquery.h"
#include "database.h"

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
            t->m_changed = false;
        }
        return Word::Ptr( t );
    }

    template <classT>
    QSharedPointer<T> getQuery( Lang src, Lang trg = QLocale::C )
    {
        Database* db = Database::instance();
        Query::Ptr q = db->findQuery( T::staticMetaObject.classname(), src, trg );
        if ( !q )
        {
            q = new T( *db, src, trg );
            db->addQuery( q );
        }
        return q->dynamicCast<T>();
    }

    WordSelectQuery::Ptr getSelectQuery( Lang src )
    {
        return getQuery<WordSelectByIdQuery>( src );
    }

    WordSelectQuery::Ptr getCreateQuery( Lang src )
    {
        return getQuery<WordCreateQuery>( src );
    }

    WordUpdateQuery::Ptr getUpdateQuery( Lang src )
    {
        return getQuery<WordUpdateQuery>( src );
    }
};

// ----------------------------------------------------------------------------

Word::Word( Lang lang )
    : m_id( 0 ), m_lang( lang ), m_changed( true )
{
}

// ----------------------------------------------------------------------------

Word::Word( const QString& text, Lang lang )
    : m_id( 0 ), m_text( text ), m_lang( lang ), m_changed( true )
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

Word::Ptr Word::find( qint64 )
{
    Word::Ptr p;
    WordQuery::Ptr qry = m_extension->query( src, trg );
    if ( qry )
    {
        qry->filterId = id;
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
    WordQuery::Ptr qry = m_extension->query( src, trg );
    if ( qry )
    {
        qry->filterId = id;
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

bool Word::save( bool withTranslations )
{
    Database* db = Database::instance();
    db->begin();

    bool ok = false;
    if ( m_changed )
    {
        if ( m_id )
        {
            // update
            WordUpdateQuery::Ptr query = m_extension->getUpdateQuery( m_src );
            query->m_text = m_text;
            ok = query->execute();
        }
        else if ( !exists( m_text ) )
        {
            // create
            WordCreateQuery::Ptr query = m_extension->getCreateQuery( m_src );
            query->m_text = m_text;
            ok = query->execute();
            if ( ok )
            {
                m_id = query->m_id;
            }
            else
            {
                db->rollback();
            }
        }
        else // duplicate
        {
            ok = false;
        }
    }

    if ( withTranslations && ok )
    {
        // TODO: handle deleted translations
        foreach( Translation::Ptr t, m_translations )
        {
            t->setSourceEntry( m_id );
            ok = t->save();
            if ( !ok ) break;
        }
    }

    if ( ok )
    {
        connect( db, SIGNAL( onTransactionFinish(bool) ), SLOT( handleTransactionFinish(bool) ) );
        return db->commit();
    }
    else
    {
        rollback();
        return false;
    }
}

// ----------------------------------------------------------------------------

void Word::handleTransactionFinish( bool success )
{
    m_changed = !success;
    disconnect( db, SIGNAL( onTransactionFinish(bool) ), SLOT( handleTransactionFinish(bool) ) );
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
