#include "itembase.h"

// ----------------------------------------------------------------------------

ItemBase::ItemBase( Lang src, Lang trg )
    : m_id( 0 ), m_srcLang( src ), m_trgLang( trg ), m_changed( 0 )
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
            // update
            UpdateQuery::Ptr query = m_extension->getUpdateQuery( m_srcLang, m_trgLang );
            writeToRecord( query );
            ok = query->execute();
        }
        else if ( !exists( m_text ) )
        {
            // create
            CreateQuery::Ptr query = m_extension->getCreateQuery( m_srcLang, m_trgLang );
            writeToRecord( query );
            ok = query->execute();
            if ( ok )
            {
                m_id = query->m_id;
            }
        }
        else // duplicate
        {
            ok = false;
        }
    }

    if ( withAssociates && ok )
    {
        ok = doSaveAssociates();
        // TODO: handle deleted translations
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

    Query::Ptr query = m_extension->getDeleteQuery( id, m_srcLang, m_trgLang );
    ok = query->execute();

    if ( ok )
    {
        ok = doDestroyAssociates();
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
    m_changed = !success;
    disconnect( db, SIGNAL( onTransactionFinish(bool) ), SLOT( handleTransactionFinish(bool) ) );
}

