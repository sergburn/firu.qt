#include "translation.h"
#include "itemextensionbase.h"
#include "translationquery.h"

// ----------------------------------------------------------------------------

class TranslationExtension : public ItemExtensionBase
{
public:
    static Translation::Ptr createFromQuery( const TranslationQuery& query )
    {
        Translation* t = new Translation( query.source, query.target );
        if ( t )
        {
            t->m_id = query.record().id;
            t->m_sid = query.record().sid;
            t->m_fmark = query.record().fmark;
            t->m_rmark = query.record().rmark;
            t->m_text = query.record().text;
        }
        return (Translation::Ptr) t;
    }

    TranslationQuery::Ptr query( Lang src, Lang trg )
    {
        if ( !m_query )
        {
            m_query = DbSchema::getTranslationQuery( src, trg );
        }
        return m_query;
    }
};

// ----------------------------------------------------------------------------

Translation::Translation( Lang src, Lang trg )
    : ItemBase( src, trg )
{
    m_extension = new TranslationExtension();
}

// ----------------------------------------------------------------------------

Translation::Translation( qint64 sid, const QString& text, Lang src, Lang trg )
    : m_sid( sid ), m_text( text ), m_srcLang( src ), m_trgLang( trg )
{
}

// ----------------------------------------------------------------------------

Translation::~Translation()
{
    delete m_extension;
}

// ----------------------------------------------------------------------------

Translation::Ptr Translation::find( qint64 id )
{
    Translation::Ptr p;
    TranslationQuery::Ptr qry = m_extension->query( src, trg );
    if ( qry )
    {
        qry->filterId = id;
        qry->start();
        if ( qry->next() )
        {
            p = TranslationUtility::createFromQuery( *qry );
        }
        delete qry;
    }
    return p;
}

// ----------------------------------------------------------------------------

Translation::List Translation::findBySourceEntry( qint64 sid, Lang src, Lang trg )
{
    Translation::List list;
    TranslationQuery::Ptr qry = m_extension->query( src, trg );
    if ( qry )
    {
        qry->filterSid = sid;
        qry->start();
        while ( qry->next() )
        {
            Translation::Ptr t = TranslationUtility::createFromQuery( *qry );
            list.append( t );
        }
        delete qry;
    }
    return list;
}

// ----------------------------------------------------------------------------

TranslationExtension& Translation::extension()
{
    return *( dynamic_cast<TranslationExtension>( m_extension ) );
}
