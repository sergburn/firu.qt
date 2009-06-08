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
    Translation( Lang src, Lang trg );
    Translation( qint64 sid, const QString& text, Lang src, Lang trg );

    QString getText() const { return m_text; }
    void setText( const QString& text );

    Mark fmark() { return m_fmark; }
    Mark rmark() { return m_rmark; }

    Translation* find( qint64 id );
    List findBySourceEntry( qint64 sid );

    static Translation* find( qint64 id, Lang src, Lang trg );
    static List findBySourceEntry( qint64 sid, Lang src, Lang trg );

    static bool saveMarks();

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
