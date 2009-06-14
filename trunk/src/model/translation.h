#ifndef TRANSLATION_H
#define TRANSLATION_H

#include <QList>
#include <QSharedPointer>

#include "model.h"
#include "itembase.h"

class TranslationExtension;

class Translation : public ItemBase
{
public:
    typedef QSharedPointer<Translation> Ptr;
    typedef QList<Ptr> List;

public:
    Translation( LangPair langs );
    Translation( qint64 sid, const QString& text, LangPair langs );

    qint64 getSid() const { return m_sid; }

    QString getText() const { return m_text; }
    void setText( const QString& text );

    Mark& fmark() { return m_fmark; }
    Mark& rmark() { return m_rmark; }

    const Mark& fmark() const { return m_fmark; }
    const Mark& rmark() const { return m_rmark; }

    Translation::Ptr find( qint64 id );
    List findBySourceEntry( qint64 sid );

    static Translation::Ptr find( qint64 id, LangPair langs );
    static List findBySourceEntry( qint64 sid, LangPair langs );

    /** Marks this translation as ToLearn */
    bool addToUserDict();

    /** Marks all translations of the sid as ToLearn */
    static bool addToUserDict( qint64 sid, LangPair langs );

private:
    Translation();
    Q_DISABLE_COPY( Translation );

    TranslationExtension& extension();

private:
    qint64 m_sid;
    QString m_text;
    Mark m_fmark;
    Mark m_rmark;

    friend class TranslationExtension;
};

#endif // TRANSLATION_H
