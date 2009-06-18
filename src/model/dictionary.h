#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <QList>
#include <QSharedPointer>
#include <QString>
#include <QObject>

#include "model.h"
#include "word.h"
#include "translation.h"

// ----------------------------------------------------------------------------

class Dictionary : public QObject
{
    Q_OBJECT

public:
    typedef QSharedPointer<Dictionary> Ptr;
    typedef QList<Ptr> List;

public:
    Dictionary( LangPair langs, QObject* parent = NULL );
    bool open();

    Lang source() { return m_langs.first; }
    Lang target() { return m_langs.second; }

    int addWord( const QString& word, const QStringList& translations );
    static Dictionary::Ptr import( const QString& file );

    Word::List findWords( const QString& pattern, TextMatch match = StartsWith, int limit = 0 );
    Translation::List findTranslations( qint64 sid );
    Translation::List findTranslations( const QString& pattern, TextMatch match = StartsWith, int limit = 0 );

    /** Adds all word's translations to learning set, i.e. marks them as ToLearn */
    bool setWordToLearn( qint64 id );

    bool addToUserDict( qint64 sid );

signals:
    void progress( double value );

private:
    Dictionary();
    Q_DISABLE_COPY( Dictionary );

private:
    LangPair m_langs;
};

#endif // DICTIONARY_H
