#include "translation.h"
#include "translationquery.h"
#include "word.h"
#include "database.h"
#include "../firudebug.h"

// ----------------------------------------------------------------------------

class TranslationQueryAdapter
{
public:
    static Translation::Ptr createFromQuery( const TranslationsQuery& query, LangPair langs )
    {
        Translation* t = new Translation( langs );
        if ( t )
        {
            t->m_id = query.record().id;
            t->m_sid = query.record().sid;
            t->m_fmark = Mark( (Mark::MarkValue) query.record().fmark );
            t->m_rmark = Mark( (Mark::MarkValue) query.record().rmark );
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
};

// ----------------------------------------------------------------------------

Translation::Translation( LangPair langs )
    : ItemBase( langs ), m_sid( 0 )
{
//    assert( langs.first != QLocale::C );
//    assert( langs.second != QLocale::C );
}

// ----------------------------------------------------------------------------

Translation::Translation( const Word::Ptr word,  const QString& text, Lang trg )
:   ItemBase( LangPair( word->getSource(), trg ) ),
    m_sid( word->getId() ),
    m_text( text )
{
//    assert( m_srcLang != QLocale::C );
//    assert( m_trgLang != QLocale::C );
    m_parent = word.toWeakRef();
}

// ----------------------------------------------------------------------------

Translation::Ptr Translation::find( qint64 id, LangPair langs )
{
    Translation::Ptr p;
    TranslationByIdQuery::Ptr qry = Database::getQuery<TranslationByIdQuery>( langs );
    if ( qry )
    {
        qry->setPrimaryKey( id );
        qry->start();
        if ( qry->next() )
        {
            p = TranslationQueryAdapter::createFromQuery( *qry, langs );
        }
    }
    return p;
}

// ----------------------------------------------------------------------------

Translation::List Translation::find(
    const QString& pattern,
    LangPair langs,
    TextMatch match,
    int limit )
{
    List list;
    TranslationsByPatternQuery::Ptr qry = Database::getQuery<TranslationsByPatternQuery>( langs );
    if ( qry )
    {
        qry->setPattern( pattern, match );
        if ( qry->start() )
        {
            while( qry->next() )
            {
                Ptr t = TranslationQueryAdapter::createFromQuery( *qry, langs );
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
    TranslationsBySidQuery::Ptr qry = Database::getQuery<TranslationsBySidQuery>( langs );
    if ( qry )
    {
        qry->setSourceEntry( sid );
        qry->start();
        while ( qry->next() )
        {
            Translation::Ptr t = TranslationQueryAdapter::createFromQuery( *qry, langs );
            list.append( t );
        }
    }
    return list;
}

// ----------------------------------------------------------------------------

bool Translation::doUpdate()
{
    if ( m_parent ) m_sid = m_parent.toStrongRef()->getId();
    if ( !m_sid ) return false;

    TranslationUpdateQuery::Ptr query = Database::getQuery<TranslationUpdateQuery>( getLangs() );
    TranslationQueryAdapter::setToQuery( query.data(), *this );
    return query->execute();
}

// ----------------------------------------------------------------------------

bool Translation::doInsert()
{
    if ( m_parent ) m_sid = m_parent.toStrongRef()->getId();
    if ( !m_sid ) return false;

    TranslationInsertQuery::Ptr query = Database::getQuery<TranslationInsertQuery>( getLangs() );
    TranslationQueryAdapter::setToQuery( query.data(), *this );
    if ( query->execute() )
    {
        m_id = query->record().id;
        return true;
    }
    return false;
}

// ----------------------------------------------------------------------------

bool Translation::doDelete()
{
    return false;
}

// ----------------------------------------------------------------------------

bool Translation::addToUserDict( qint64 sid, LangPair langs )
{
    UpdateMarksQuery::Ptr query = Database::getQuery<UpdateMarksQuery>( langs );

    if ( query )
    {
        Mark mark;
        mark.restart();
        query->record().fmark = mark();
        query->record().rmark  = mark();
        query->record().sid = sid;
        return query->execute();
    }
    else
    {
        qDebug() << "Couldn't get UpdateMarksQuery";
        return false;
    }
}
