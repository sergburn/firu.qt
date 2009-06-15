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

    Lang source() { return m_langs.first; }
    Lang target() { return m_langs.second; }

    bool addWord( const QString& word, const QStringList& translations );
    static Dictionary::Ptr import( const QString& file );

    Word::List findWords( const QString& pattern, TextMatch match = StartsWith, int limit = 0 );
    Translation::List findTranslations( qint64 sid );
    Translation::List findTranslations( const QString& pattern, TextMatch match = StartsWith, int limit = 0 );

    /** Adds all word's translations to learning set, i.e. marks them as ToLearn */
    bool setWordToLearn( qint64 /*id*/ )
    {
//        // drop marks for these translations
//        foreach( Translation t, m_translations )
//        {
//            t.fmark().restart();
//            t.rmark().restart();
//        }
//        Translation::saveMarks( m_translations, m_id );
        return false;
    }

    bool addToUserDict( qint64 sid );

private:
    Dictionary();
    Q_DISABLE_COPY( Dictionary );

private:
    LangPair m_langs;
};

#endif // DICTIONARY_H
