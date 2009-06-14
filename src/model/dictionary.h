#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <QList>
#include <QSharedPointer>

#include "model.h"
#include "word.h"
#include "translation.h"

// ----------------------------------------------------------------------------

class Dictionary
{
public:
    typedef QSharedPointer<Dictionary> Ptr;
    typedef QList<Ptr> List;

public:
    Dictionary( LangPair langs );
    bool open();

    Lang source() { return m_vocab.source(); }
    Lang target() { return m_trans.target(); }

    bool addWord( const QString& word, const QStringList& translations );
    static Dictionary::Ptr import( const QString& file );

    Word::List findWords( const QString& pattern, TextMatch match = StartsWith );
    Translation::List findTranslations( const QString& pattern, TextMatch match = StartsWith );

    /** Adds all word's translations to learning set, i.e. marks them as ToLearn */
    bool setWordToLearn( qint64 id )
    {
        // drop marks for these translations
        foreach( Translation t, m_translations )
        {
            t.fmark().restart();
            t.rmark().restart();
        }
        Translation::saveMarks( m_translations, m_id );
    }

private:
    Dictionary();
    Q_DISABLE_COPY( Dictionary );

private:
    Lang m_srcLang;
    Lang m_trgLang;
};

#endif // DICTIONARY_H
