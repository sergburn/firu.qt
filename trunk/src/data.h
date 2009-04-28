#ifndef DATA_H
#define DATA_H

#include <QObject>
#include <QList>
#include <QStringList>
#include <QLocale>

typedef QLocale::Language Lang;

class Word
{
public:
    Word();
    Word( int id, const QString& text );

    const QString& getText() const;
    int getId() const;

private:
    int m_id;
    QString m_text;
};

class Translation
{
public:
    enum Rate {
        Unknown = 0,
        ToLearn = 1,
        Good    = 2,
        Better  = 3,
        Learned = 4
    };

    Translation();
    Translation( int id, const QString& text, const Word& source, const Word& target );

    const Word& getSource() const;
    const Word& getTarget() const;

    int getId();
    Rate getRate() const;
    
private:
    int m_id;
    Word m_source;
    Word m_target;
    int m_rate;
};

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
    static Data& instance();

    QList<Lang> getLanguages() const;
    int getNumEntries( Lang lang );

public slots:
    void setSourceLanguage( Lang lang );
    void setTargetLanguage( Lang lang );
    void reverseLanguages();

public:
    QList<Word> searchWords( const QString& pattern );
    QList<Translation> getTranslations( const Word& word );

    Exercise createExercise();
    
signals:
    void sourceLangChanged( Lang );
    void targetLangChanged( Lang );
    
private:
    bool addTranslation( Lang source, Lang target );
    bool addLanguage( Lang lang );
    
    bool isLangExists( Lang lang );
    bool isTransExists( Lang source, Lang target );
    
private:
    
    Lang m_source_lang;
    Lang m_target_lang;
    
    DbSchema* m_schema;
};

#endif // DATA_H
