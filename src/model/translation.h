#ifndef TRANSLATION_H
#define TRANSLATION_H

#include <QList>
#include <QSharedPointer>

#include "model.h"

class TranslationExtension;

class Translation
{
public:
    typedef QSharedPointer<Translation> Ptr;
    typedef QList<Ptr> List;

public:
    Translation( Lang src, Lang trg );
    Translation( qint64 sid, const QString& text, Lang src, Lang trg );

    QString getText() const { return m_text; }
    qint64 getId() const { return m_id; }

    Mark fmark() { return m_fmark; }
    Mark rmark() { return m_rmark; }

    Translation* find( qint64 id );
    List findBySourceEntry( qint64 sid );

    static Translation* find( qint64 id, Lang src, Lang trg );
    static List findBySourceEntry( qint64 sid, Lang src, Lang trg );

private:
    Translation();
    Q_DISABLE_COPY( Translation );

private:
    qint64 m_id;
    qint64 m_sid;
    QString m_text;
    Mark m_fmark;
    Mark m_rmark;
    Lang m_srcLang;
    Lang m_trgLang;

    friend class TranslationExtension;
    TranslationExtension* m_extension;
};

#endif // TRANSLATION_H
