#ifndef WORD_H
#define WORD_H

#include <QList>
#include <QMap>
#include <QString>
#include <QSharedPointer>

#include "model.h"
#include "itembase.h"
#include "translation.h"

class WordQueryAdapter;

class Word : public ItemBase
{
public:
    typedef QSharedPointer<Word> Ptr;
    typedef QList<Ptr> List;

public:
    Word( const QString& text, Lang lang );

    static Ptr find( qint64 id, Lang src );
    static List find( const QString& pattern, Lang lang, TextMatch match = StartsWith, int limit = 0 );
    static bool exists( const QString& pattern, Lang lang );

    static List filter( const List& list, const QString& pattern, TextMatch match = StartsWith );

public:
    void setText( const QString& text );

    bool match( const QString& pattern, TextMatch match );

    Translation::List translations( Lang trg );
    bool addTranslation( const Translation& trans );
    bool addTranslation( const QString& text, Lang trg );

private:
    Word();
    Word( Lang lang );
    Q_DISABLE_COPY( Word )

    virtual bool doInsert();
    virtual bool doUpdate();
    virtual bool doDelete();
    virtual bool doSaveAssociates();
    virtual bool doDeleteAssociates();

private slots:
    void handleTransactionFinish( bool success );

private:
    QMap<Lang, Translation::List> m_translations;

    friend class WordQueryAdapter;
};

#endif // WORD_H
