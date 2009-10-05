#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <QList>
#include <QSharedPointer>
#include <QString>
#include <QObject>

#include "model.h"
#include "word.h"
#include "translation.h"
#include "exercise.h"

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
    int count( const QString& pattern );

    /** Adds all word's translations to learning set, i.e. marks them as ToLearn */
    bool setWordToLearn( qint64 id );

    bool addToUserDict( qint64 sid );
    
    /** Marks all translations as Unknown */
    bool clearLearningSet();

    Exercise::Ptr createReverseExercise();

signals:
    void progress( double value );

private:
    Dictionary();
    Q_DISABLE_COPY( Dictionary )

private:
    LangPair m_langs;
};

#endif // DICTIONARY_H
