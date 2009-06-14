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
    Word( Lang lang ) : m_id( 0 ), m_lang( lang ) {}
    Word( const QString& text, Lang lang ) : m_id( 0 ), m_text( text ), m_lang( lang ) {}

    Ptr find( qint64 id );
    List find( const QString& pattern, TextMatch match = StartsWith, int limit = 0 );
    bool Word::exists( const QString& pattern );

    static Ptr create( const QString& text, Lang lang );
    static Ptr find( Lang lang, qint64 id );
    static List find( Lang lang, const QString& pattern, TextMatch match = StartsWith );
    static List filter( const List& list, const QString& pattern, TextMatch match = StartsWith );

    bool loadTranslations();

    bool save( bool withTranslations = true );

public:
    const QString& getText() const { return m_text; }
    void setText( const QString& text );

    Translation::List translations( Lang trg );
    bool addTranslation( const Translation& trans );
    bool addTranslation( const QString& text, Lang trg );

private:
    Word();
    Q_DISABLE_COPY( Word );

    virtual bool doSaveAssociates();

private slots:
    void handleTransactionFinish( bool success );

private:
    QString m_text;
    QMap<Lang, Translation::List> m_translations;

    bool m_changed;

    friend class WordExtension;
};

#endif // WORD_H
