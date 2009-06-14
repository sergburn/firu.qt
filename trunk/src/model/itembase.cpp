#include "itembase.h"

// ----------------------------------------------------------------------------

ItemBase::ItemBase( Lang src )
    : m_id( 0 ), m_srcLang( src ), m_trgLang( QLocale::C ), m_changed( 0 )
{
}

// ----------------------------------------------------------------------------

ItemBase::ItemBase( LangPair langs )
    : m_id( 0 ), m_srcLang( langs.first ), m_trgLang( langs.second ), m_changed( 0 )
{
}

// ----------------------------------------------------------------------------

Word::Ptr ItemBase::load( qint64 id )
{
    Word::Ptr p;
    WordQuery::Ptr qry = m_extension->getSelectQuery( id, m_srcLang, m_trgLang );
    if ( qry && qry->start() && qry->next() )
    {
        p = m_extension->createFromQuery( qry );
    }
    return p;
}

// ----------------------------------------------------------------------------

bool ItemBase::save( bool withAssociates )
{
    Database* db = Database::instance();
    db->begin();

    bool ok = false;
    if ( m_changed )
    {
        if ( m_id )
        {
            ok = doUpdate();
        }
        else if ( !exists( m_text ) )
        {
            ok = doCreate();
        }
        else // duplicate
        {
            ok = false;
        }
    }

    if ( withAssociates && ok )
    {
        ok = doSaveAssociates();
    }

    if ( ok )
    {
        connect( db, SIGNAL( onTransactionFinish(bool) ), SLOT( handleTransactionFinish(bool) ) );
        return db->commit();
    }
    else
    {
        db->rollback();
        return false;
    }
}

// ----------------------------------------------------------------------------

void ItemBase::destroy( qint64 id )
{
    Database* db = Database::instance();
    db->begin();

    doDelete();

    if ( ok )
    {
        ok = doDeleteAssociates();
    }

    if ( ok )
    {
        connect( db, SIGNAL( onTransactionFinish(bool) ), SLOT( handleTransactionFinish(bool) ) );
        return db->commit();
    }
    else
    {
        db->rollback();
        return false;
    }
}

// ----------------------------------------------------------------------------

void ItemBase::handleTransactionFinish( bool success )
{
    m_changed = success ? 0 : m_changed;
    disconnect( db, SIGNAL( onTransactionFinish(bool) ), SLOT( handleTransactionFinish(bool) ) );
}

