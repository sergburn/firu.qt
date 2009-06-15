#ifndef ITEMEXTENSIONBASE_H
#define ITEMEXTENSIONBASE_H

#include "model.h"
#include "database.h"

class ItemExtensionBase
{
public:
    template <classT>
    static QSharedPointer<T> getQuery( Lang src )
    {
        assert( src != QLocale::C );

        Database* db = Database::instance();
        Query::Ptr q = db->findQuery( T::staticMetaObject.classname(), src );
        if ( !q )
        {
            q = new T( db, src, db );
            db->addQuery( q );
        }
        return q->dynamicCast<T>();
    }

    template <classT>
    static QSharedPointer<T> getQuery( LangPair langs )
    {
        assert( langs.first != QLocale::C );
        assert( langs.second != QLocale::C );

        Database* db = Database::instance();
        Query::Ptr q = db->findQuery( T::staticMetaObject.classname(), langs.first, langs.second );
        if ( !q )
        {
            q = new T( db, langs, db );
            db->addQuery( q );
        }
        return q->dynamicCast<T>();
    }
};

#endif // ITEMEXTENSIONBASE_H
