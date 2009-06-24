#ifndef ITEMBASE_H
#define ITEMBASE_H

#include <QObject>
#include "model.h"

class ItemBase : public QObject
{
    Q_OBJECT
public:
    qint64 id() const { return m_id; }
    const QString& text() const { return m_text; }

    LangPair getLangs() const { return LangPair( m_srcLang, m_trgLang ); }
    Lang getSource() const { return m_srcLang; }
    Lang getTarget() const { return m_trgLang; }

    bool save( bool withAssociates = true );
    bool destroy();

protected:
    virtual bool doInsert() = 0;
    virtual bool doUpdate() = 0;
    virtual bool doSaveAssociates() { return true; }
    virtual bool doDelete() = 0;
    virtual bool doDeleteAssociates() { return true; }

protected:
    ItemBase( Lang src );
    ItemBase( LangPair langs );
    ItemBase( const QString& text, Lang src );
    ItemBase( const QString& text, LangPair langs );

private:
    ItemBase();
    Q_DISABLE_COPY( ItemBase );

private slots:
    void handleTransactionFinish( bool success );

protected:
    qint64 m_id;
    QString m_text;

    Lang m_srcLang;
    Lang m_trgLang;
    qint32 m_changed;
};

#endif // ITEMBASE_H
