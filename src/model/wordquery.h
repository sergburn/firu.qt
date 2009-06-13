#ifndef WORDQUERY_H
#define WORDQUERY_H

#include <QSharedPointer>
#include "query.h"

class WordsQuery : public Query
{
public:
    typedef QSharedPointer<WordsQuery> Ptr;

    WordsQuery( sqlite* db, Lang src, QObject* parent = NULL );

    class Record
    {
        qint64 id;
        QString text;
    };
    Record& record();
    const Record& record() const;

protected: // from Query
    virtual QString buildSql() const;
    virtual int bind();
    virtual void read();

private:
    WordsQuery();
    Q_DISABLE_COPY( WordsQuery );

protected:
    Record m_record;
};

// ----------------------------------------------------------------------------

class WordByIdQuery : public WordsQuery
{
public:
    WordByIdQuery( sqlite* db, Lang src, QObject* parent = NULL )
        : WordsQuery( db, src, parent ) {}

protected: // from Query
    virtual QString buildSql() const;
    virtual int bind();
};

// ----------------------------------------------------------------------------

class WordsByPatternQuery : public WordsQuery
{
public:
    WordsByPatternQuery( sqlite* db, Lang src, QObject* parent = NULL );
    void setPattern( const QString& pattern, TextMatch match );

protected: // from Query
    virtual QString buildSql() const;
    virtual int bind();

private:
    QString m_pattern;
    TextMatch m_match;
};

// ----------------------------------------------------------------------------

class WordUpdateQuery : public WordsQuery
{
public:
    WordUpdateQuery( sqlite* db, Lang src, QObject* parent = NULL );

    virtual int execute();

protected: // from Query
    virtual QString buildSql() const;
    virtual int bind();
};

// ----------------------------------------------------------------------------

class WordInsertQuery : public WordUpdateQuery
{
public:
    WordInsertQuery( sqlite* db, Lang src, QObject* parent = NULL );

    virtual int execute();

protected: // from Query
    virtual QString buildSql() const;
    virtual bool execute();
};

#endif // WORDQUERY_H
