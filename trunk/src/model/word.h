#ifndef WORD_H
#define WORD_H

#include <QList>
#include <QSharedPointer>

#include "model.h"
#include "itembase.h"
#include "translation.h"

class WordExtension;

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

    bool save( bool withTranslations = true );

public:
    const QString& getText() const { return m_text; }
    void setText( const QString& text );

    Translation::List translations( Lang trg );
    bool addTranslation( const Translation& trans );
    bool addTranslation( const QString& text, Lang trg );

private:
    Word();
    Word( Lang lang );
    Q_DISABLE_COPY( Word );

    virtual bool doSaveAssociates();

private slots:
    void handleTransactionFinish( bool success );

private:
    QString m_text;
    QMap<Lang, Translation::List> m_translations;

    friend class WordExtension;
};

#endif // WORD_H
