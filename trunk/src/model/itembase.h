#ifndef ITEMBASE_H
#define ITEMBASE_H

#include "model.h"

class ItemBase
{
public:
    ItemBase( Lang src, Lang trg = QLocale::C );

    qint64 getId() const { return m_id; }

    bool load( qint64 id );

    bool save( bool withAssociates = true );

    void destroy() { destroy( m_id ); }
    static void destroy( qint64 id );

protected:
    virtual bool doInsert() = 0;
    virtual bool doUpdate() = 0;
    virtual bool doSaveAssociates() {}
    virtual bool doDelete() = 0;
    virtual bool doDeleteAssociates() {}

private:
    ItemBase();
    Q_DISABLE_COPY( ItemBase );

private slots:
    void handleTransactionFinish( bool success );

protected:
    qint64 m_id;
    Lang m_srcLang;
    Lang m_trgLang;
    qint32 m_changed;

    ItemExtensionBase* m_extension;
};

#endif // ITEMBASE_H
