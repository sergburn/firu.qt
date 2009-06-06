#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <QList>
#include <QSharedPointer>

#include "model.h"
#include "word.h"
#include "translation.h"

// ----------------------------------------------------------------------------

class Vocabulary
{
public:
    Vocabulary( Lang src );

    Ptr findWord( qint64 id );
    List findWords( const QString& pattern, TextMatch match = StartsWith );

private:
    Vocabulary();
    Q_DISABLE_COPY( Vocabulary );

private:
    Lang m_srcLang;
};

// ----------------------------------------------------------------------------

class Translations
{
public:
    Translations( Lang src, Lang trg );

    Translation* find( qint64 id );
    List findBySourceEntry( qint64 sid );

private:
    Translations();
    Q_DISABLE_COPY( Translations );

private:
    Lang m_srcLang;
    Lang m_trgLang;
};

// ----------------------------------------------------------------------------

class Dictionary
{
public:
    typedef QSharedPointer<Dictionary> Ptr;
    typedef QList<Ptr> List;

public:
    Dictionary( Lang src, Lang trg );

    Vocabulary& vocabulary() { return m_vocab; }
    Translations& translations() { return m_trans; }

private:
    Dictionary();
    Q_DISABLE_COPY( Dictionary );

private:
    Vocabulary m_vocab;
    Translations m_trans;
};

#endif // DICTIONARY_H
