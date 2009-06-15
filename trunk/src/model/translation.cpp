#include "translation.h"
#include "itemextensionbase.h"
#include "translationquery.h"
#include "word.h"

// ----------------------------------------------------------------------------

class TranslationExtension : public ItemExtensionBase
{
public:
    static Translation::Ptr createFromQuery( const TranslationQuery& query )
    {
        Translation* t = new Translation( LangPair( query.source(), query.target() ) );
        if ( t )
        {
            t->m_id = query.record().id;
            t->m_sid = query.record().sid;
            t->m_fmark = query.record().fmark;
            t->m_rmark = query.record().rmark;
            t->m_text = query.record().text;
            t->m_changed = 0;
        }
        return (Translation::Ptr) t;
    }

    static void setToQuery( TranslationsQuery* query, const Translation& t )
    {
        query->record().id = t.m_id;
        query->record().sid = t.m_sid;
        query->record().text = t.m_text;
        query->record().fmark = t.m_fmark();
        query->record().rmark = t.m_rmark();
    }

    static TranslationsQuery::Ptr getSelectQuery( LangPair langs )
    {
        return getQuery<TranslationsQuery>( langs );
    }

    static TranslationByIdQuery::Ptr getSelectByIdQuery( LangPair langs )
    {
        return getQuery<TranslationByIdQuery>( langs );
    }

    static TranslationBySidQuery::Ptr getSelectBySidQuery( LangPair langs )
    {
        return getQuery<TranslationBySidQuery>( langs );
    }

    static TranslationsByPatternQuery::Ptr getSelectByPatternQuery( LangPair langs )
    {
        return getQuery<TranslationsByPatternQuery>( langs );
    }

    static TranslationInsertQuery::Ptr getInsertQuery( LangPair langs )
    {
        return getQuery<TranslationInsertQuery>( langs );
    }

    static TranslationUpdateQuery::Ptr getUpdateQuery( LangPair langs )
    {
        return getQuery<TranslationUpdateQuery>( langs );
    }
};

// ----------------------------------------------------------------------------

Translation::Translation( LangPair langs )
    : ItemBase( langs ), m_sid( 0 )
{
    assert( langs.first != QLocale::C );
    assert( langs.second != QLocale::C );
}

// ----------------------------------------------------------------------------

Translation::Translation( const Word::Ptr word,  const QString& text, Lang trg )
:   ItemBase( LangPair( word->getSource(), trg ) ),
    m_sid( word->getId() ),
    m_text( text )
{
    assert( m_srcLang != QLocale::C );
    assert( m_trgLang != QLocale::C );
    m_parent = word.toWeakRef();
}

// ----------------------------------------------------------------------------

Translation::Ptr Translation::find( LangPair langs )
{
    Translation::Ptr p;
    TranslationByIdQuery::Ptr qry = TranslationExtension::getSelectQuery( langs );
    if ( qry )
    {
        qry->setPrimaryKey( id );
        qry->start();
        if ( qry->next() )
        {
            p = TranslationExtension::createFromQuery( *qry );
        }
    }
    return p;
}

// ----------------------------------------------------------------------------

Translation::List Translation::find(
    const QString& pattern,
    LangPair langs,
    TextMatch match = StartsWith,
    int limit = 0 )
{
    List list;
    TranslationsByPatternQuery::Ptr qry = TranslationExtension::getSelectByPatternQuery( getLangs() );
    if ( qry )
    {
        qry->setPattern( pattern, match );
        if ( qry->start() )
        {
            while( qry->next() )
            {
                Ptr t = TranslationExtension::createFromQuery( *qry );
                list.append( t );
                if ( limit > 0 && list.count() >= limit )
                {
                    break;
                }
            }
        }
    }
    return list;
}

// ----------------------------------------------------------------------------

Translation::List Translation::findBySourceEntry( qint64 sid, LangPair langs )
{
    Translation::List list;
    TranslationsBySidQuery::Ptr qry = TranslationExtension::getSelectBySidQuery( langs );
    if ( qry )
    {
        qry->setSourceEntry( sid );
        qry->start();
        while ( qry->next() )
        {
            Translation::Ptr t = TranslationExtension::createFromQuery( *qry );
            list.append( t );
        }
    }
    return list;
}

// ----------------------------------------------------------------------------

bool Translation::doUpdate()
{
    if ( m_parent ) m_sid = m_parent->getId();
    if ( !m_sid ) return false;

    Query::Ptr query = TranslationExtension::getUpdateQuery( getLangs() );
    query->setToQuery( *this );
    return query->execute();
}

// ----------------------------------------------------------------------------

bool Translation::doInsert()
{
    if ( m_parent ) m_sid = m_parent->getId();
    if ( !m_sid ) return false;

    Query::Ptr query = TranslationExtension::getInsertQuery( getLangs() );
    query->setToQuery( *this );
    ok = query->execute();
    if ( ok )
    {
        m_id = query->record().id;
    }
}
