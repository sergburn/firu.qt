#ifndef TRANSLATION_H
#define TRANSLATION_H

#include <QList>
#include <QSharedPointer>

#include "model.h"
#include "itembase.h"

class Word;
class TranslationQueryAdapter;

class Translation : public ItemBase
{
public:
    typedef QSharedPointer<Translation> Ptr;
    typedef QList<Ptr> List;

public:
    Translation( const QSharedPointer<Word> word, const QString& text, Lang trg );
    ~Translation();

    qint64 getSid() const { return m_sid; }

    void setText( const QString& text );

    Mark& fmark() { return m_fmark; }
    Mark& rmark() { return m_rmark; }

    const Mark& fmark() const { return m_fmark; }
    const Mark& rmark() const { return m_rmark; }

    QSharedPointer<Word> getWord();

    static Ptr find( qint64 id, LangPair langs );
    static List find( const QString& pattern, LangPair langs, TextMatch match = StartsWith, int limit = 0 );
    static List findBySourceEntry( qint64 sid, LangPair langs );
    static List getAllWithRMark( Mark::MarkValue level, LangPair langs );
    static QList<qint64> getIdsByRMark( Mark::MarkValue level, LangPair langs );

    /** Marks this translation as ToLearn */
    bool addToUserDict();

    /** Marks all translations of the sid as ToLearn */
    static bool addToUserDict( qint64 sid, LangPair langs );

    /** Saves marks for this translation */
    bool saveMarks();

protected:
    virtual bool doInsert();
    virtual bool doUpdate();
    virtual bool doDelete();

private:
    Translation();
    Translation( LangPair langs );
    Q_DISABLE_COPY( Translation );

private:
    qint64 m_sid;
    Mark m_fmark;
    Mark m_rmark;
    QSharedPointer<Word> m_parent;

    friend class TranslationQueryAdapter;
};

#endif // TRANSLATION_H
