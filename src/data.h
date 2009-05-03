#ifndef DATA_H
#define DATA_H

#include <QObject>
#include <QList>
#include <QStringList>
#include <QLocale>

typedef QLocale::Language Lang;

class Data;

class Word
{
public:
    Word() : m_id( 0 ) {};
    Word( qint64 id, const QString& text ) : m_id( id ), m_text( text ) {};

    const QString& getText() const { return m_text; };
    qint64 getId() const { return m_id; };

private:
    qint64 m_id;
    QString m_text;
};

class Translation : public Word
{
public:
    enum Mark {
        Unknown = 0,
        ToLearn = 1,
        Good    = 2,
        Better  = 3,
        Learned = 4
    };

    Translation() : m_fmark( Unknown ), m_rmark( Unknown )  {};
    Translation( int id, const QString& text, int fmark, int rmark )
            : Word( id, text ), m_fmark( fmark ), m_rmark( rmark )  {};

    Mark getFmark() const;
    Mark getRmark() const;

private:
    int m_fmark;
    int m_rmark;
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
    bool select( Lang src, Lang trg );
    void getLanguages( Lang& src, Lang& trg ) const;

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
