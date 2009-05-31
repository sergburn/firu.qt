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

    enum MarkValue {
        Unknown = 0,
        ToLearn = 1,
        Good    = 2,
        Better  = 3,
        Learned = 4
    };

    class Mark
    {
    public:
        Mark() : m_value( Unknown ) {}

        MarkValue upgrade()
        {
            if ( m_value < Learned )
            {
                m_value = (MarkValue)((int)m_value+1);
            }
            return m_value;
        }
        MarkValue drop() { return m_value = ToLearn; }
        MarkValue reset() { return m_value = Unknown; }
        MarkValue init() { return m_value = ToLearn; }
        MarkValue operator()() const { return m_value; }

    private:
        MarkValue m_value;
    };

public:
    Translation( Lang src, Lang trg );
    Translation( qint64 sid, const QString& text, Lang src, Lang trg );

    QString getText() const { return m_text; }
    qint64 getId() const { return m_id; }

    Mark fmark() { return m_fmark; }
    Mark rmark() { return m_rmark; }

    static Translation* find( qint64 id );
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
