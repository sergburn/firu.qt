#include "itembase.h"
#include "database.h"
#include "../firudebug.h"

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

ItemBase::ItemBase( const QString& text, Lang src )
    : m_id( 0 ), m_text( text ), m_srcLang( src ), m_trgLang( QLocale::C ), m_changed( 0 )
{
}

// ----------------------------------------------------------------------------

ItemBase::ItemBase( const QString& text, LangPair langs )
    : m_id( 0 ), m_text( text ), m_srcLang( langs.first ), m_trgLang( langs.second ), m_changed( 0 )
{
}

// ----------------------------------------------------------------------------

bool ItemBase::save( bool withAssociates )
{
    Database* db = Database::instance();
    bool ok = false;
    if ( db->begin() )
    {
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
            ok = db->commit();
        }
        else
        {
            db->rollback();
        }
    }
    return ok;
}

// ----------------------------------------------------------------------------

bool ItemBase::destroy()
{
    Database* db = Database::instance();
    bool ok = false;
    if ( db->begin() )
    {
        ok = doDelete();

        if ( ok )
        {
            ok = doDeleteAssociates();
        }

        if ( ok )
        {
            connect( db, SIGNAL( onTransactionFinish(bool) ), SLOT( handleTransactionFinish(bool) ) );
            ok = db->commit();
        }
        else
        {
            db->rollback();
        }
    }
    return ok;
}

// ----------------------------------------------------------------------------

void ItemBase::handleTransactionFinish( bool success )
{
    if ( success )
        qDebug() << "Transaction completed";
    else
        qDebug() << "Transaction failed";

    m_changed = success ? 0 : m_changed;
    Database* db = Database::instance();
    disconnect( db, SIGNAL( onTransactionFinish(bool) ), this, SLOT( handleTransactionFinish(bool) ) );
}

