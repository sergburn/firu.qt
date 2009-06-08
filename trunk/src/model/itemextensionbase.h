#ifndef ITEMEXTENSIONBASE_H
#define ITEMEXTENSIONBASE_H

class ItemExtensionBase
{
public:
    template <classT>
    static QSharedPointer<T> getQuery( Lang src, Lang trg = QLocale::C )
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
};

#endif // ITEMEXTENSIONBASE_H
