#ifndef DATA_H
#define DATA_H

#include <QObject>
#include <QList>
#include <QStringList>
#include <QLocale>

#include "model/model.h"

typedef QLocale::Language Lang;

class Data;

class TranslationTest
{
    friend class Data;
public:
    enum AnswerValue { NoMatch, PartialMatch, FullMatch };

    const QString& getQuestion() const;
    QStringList getHints();
    AnswerValue checkAnswer( const QString& answer );
    const QString& getAnswer();

    enum TestResult {
        NotAsked,
        Passed,             // adds 1 to current rate
        PassedWithHints,    // doesn't change rate if 1 or 2, demotes rate 3 to 2
        Failed              // sets current rate 1.
    };

    TestResult getResult() const;

private:
    Translation m_challenge;
    TestResult m_result;
    QStringList m_hints;
};

class Exercise
{
    friend class Data;
public:    
    TranslationTest getFirstTest();
    TranslationTest getNextTest();

    struct Stats
    {
        int currentTest;
        int totalTests;
        int passed;
    };

    Stats getStats() const;
    
private:
    Exercise( Data& );
    
private:
    QList<Exercise> m_tasks;
};

class DbSchema;

class Data : public QObject
{
    Q_OBJECT

public:
    Data();
    ~Data();
    bool open();
    bool select( Lang src, Lang trg );
    void getLanguages( Lang& src, Lang& trg ) const;
    Lang source() { return m_source_lang; }
    Lang target() { return m_target_lang; }

    int getNumEntries( Lang lang );
    int getNumTranslations( Lang src, Lang trg );

    // getStatistics();
    // clearResults();
    
    bool importDictionary( const QString& file );

public slots:
    void setSourceLanguage( Lang lang );
    void setTargetLanguage( Lang lang );

public:
    Word getWord( qint64 id );
    QList<Word> searchWords( const QString& pattern );
    QList<Word> searchTranslations( const QString& pattern );
    QList<Translation> getTranslations( const Word& word );

    Exercise createExercise();
    
signals:
    void sourceLangChanged( Lang );
    void targetLangChanged( Lang );
    void progress( int value );
    
private:
    bool addLanguage( Lang lang );
    int addTranslations( Lang src, Lang trg, const QString& source, const QStringList& targets );

    bool isLangExists( Lang lang );
    bool isTransExists( Lang source, Lang target );
    
private:
    
    Lang m_source_lang;
    Lang m_target_lang;
    
    DbSchema* m_schema;
};

#endif // DATA_H
