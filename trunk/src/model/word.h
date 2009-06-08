#ifndef WORD_H
#define WORD_H

#include <QList>
#include <QSharedPointer>

#include "model.h"
#include "translation.h"

class WordExtension;

class Word
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
    qint64 getId() const { return m_id; }

    void setText( const QString& text );

    Translation::List translations();
    bool addTranslation( const Translation& trans );
    bool addTranslation( const QString& text, Lang trg );

private:
    Word();
    Q_DISABLE_COPY( Word );

private slots:
    void handleTransactionFinish( bool success );

private:
    qint64 m_id;
    QString m_text;
    Translation::List m_translations;
    Lang m_lang;

    bool m_changed;

    friend class WordExtension;
    WordExtension* m_extension;
};

#endif // WORD_H
