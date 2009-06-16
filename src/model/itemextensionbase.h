#ifndef ITEMEXTENSIONBASE_H
#define ITEMEXTENSIONBASE_H

#include "model.h"
#include "database.h"

class ItemExtensionBase
{
public:
    template <class T>
    static QSharedPointer<T> getQuery( Lang src )
    {
//        assert( src != QLocale::C );

        Database* db = Database::instance();
        Query::Ptr q = db->findQuery( T::staticMetaObject.className(), src );
        if ( !q )
        {
            Query::Ptr tq( new T( db, src, db ) );
            db->addQuery( tq );
            q = tq;
        }
        return q.dynamicCast<T>();
    }

    template <class T>
    static QSharedPointer<T> getQuery( LangPair langs )
    {
//        assert( langs.first != QLocale::C );
//        assert( langs.second != QLocale::C );

        Database* db = Database::instance();
        Query::Ptr q = db->findQuery( T::staticMetaObject.className(), langs.first, langs.second );
        if ( !q )
        {
            Query::Ptr tq( new T( db, langs, db ) );
            db->addQuery( tq );
            q = tq;
        }
        return q.dynamicCast<T>();
    }
};

#endif // ITEMEXTENSIONBASE_H
