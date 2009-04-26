#ifndef DATA_H
#define DATA_H

#include <QObject>
#include <QList>
#include <QLocale>

typedef Lang QLocale::Language;

class Word
{
public:
    Word();

    const QString& getText() const;
    int getId() const;
};

class Translation
{
public:
    Translation();

    const QString& getText() const;
    int getRate() const;
    enum {
        Unknown = 0,
        ToLearn = 1,

    };

    int getSourceId() const;
    int getTargetId() const;
};

class TranslationTest
{
public:
    enum AnswerValue { NoMatch, PartialMatch, FullMatch };

    const QString& getQuestion() const;
    QStringList getHints();
    AnswerValue checkAnswer( const QString& answer );
    const QString& getAnswer();

    enum TestResult {
        NotAsked = -1000,
        Passed = 1,             // adds 1 to current rate
        PassedWithHints = 0,    // doesn't change current rate
        Failed = -1             // sets current rate 0.
    };

    TestResult getResult() const;

private:
    enum TestState { Initial, HintsGiven, AnswerGiven, PassedWithHints, Passed } iState;
};

class Exercise
{
    TranslationTest getFirstTest();
    TranslationTest getNextTest();

    struct Stats
    {
        int currentTest;
        int totalTests;
        int passed;
    };

    Stats getStats() const;
};

class Data : public QObject
{
    Q_OBJECT

public:
    Data();

    QList<Lang> getLanguages() const;
    int getNumEntries( Lang lang );

public slots:
    bool setSourceLanguage( Lang lang );
    bool setTargetLanguage( Lang lang );
    void reverseLanguages();

public:
    QList<Word> searchWords( const QString& pattern );
    QList<Translation> getTranslations( const Word& word );

};

#endif // DATA_H
