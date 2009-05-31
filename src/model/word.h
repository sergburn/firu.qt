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

    static Word* create();
    static Word* find_exact( QString );
    static Word* load( qint64 );

    bool save( bool withTranslations = true );

public:
    const QString& getText() const { return m_text; }
    qint64 getId() const { return m_id; }

    Translation::List translations();
    bool addTranslation( QString );

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
