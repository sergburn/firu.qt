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

    static Ptr find( Lang lang, qint64 );
    static List find( Lang lang, const QString& pattern, TextMatch match = StartsWith );
    static List filter( Lang lang, const List& list, const QString& pattern, TextMatch match = StartsWith );

    bool save( bool withTranslations = true );

public:
    const QString& getText() const { return m_text; }
    qint64 getId() const { return m_id; }

    Translation::List translations();
    bool addTranslation( const Translation& trans );

private:
    Word();
    Q_DISABLE_COPY( Word );

private:
    qint64 m_id;
    QString m_text;
    Translation::List m_translations;
    Lang m_lang;

    friend class WordExtension;
    WordExtension* m_extension;
};

#endif // WORD_H
