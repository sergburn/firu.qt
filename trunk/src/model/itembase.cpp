#include "itembase.h"
#include "database.h"

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

bool ItemBase::save( bool withAssociates )
{
    Database* db = Database::instance();
    if ( db->begin() )
    {
        bool ok = false;
        if ( m_changed )
        {
            if ( m_id )
            {
                ok = doUpdate();
            }
            else
            {
                ok = doInsert();
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
    return false;
}

// ----------------------------------------------------------------------------

bool ItemBase::destroy()
{
    Database* db = Database::instance();
    if ( db->begin() )
    {
        bool ok = doDelete();

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
    return false;
}

// ----------------------------------------------------------------------------

void ItemBase::handleTransactionFinish( bool success )
{
    m_changed = success ? 0 : m_changed;
    Database* db = Database::instance();
    disconnect( db, SIGNAL( onTransactionFinish(bool) ), this, SLOT( handleTransactionFinish(bool) ) );
}

