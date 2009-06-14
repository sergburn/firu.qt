#ifndef WORDQUERY_H
#define WORDQUERY_H

#include <QSharedPointer>
#include "query.h"

class WordsQuery : public Query
{
public:
    typedef QSharedPointer<WordsQuery> Ptr;

    WordsQuery( Database* db, Lang src, QObject* parent = NULL );

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

class WordsCountQuery : public WordsQuery
{
public:
    WordsCountQuery( Database* db, Lang src, QObject* parent = NULL )
        : WordsQuery( db, src, parent ) {}

protected: // from Query
    virtual QString buildSql() const;
};

// ----------------------------------------------------------------------------

class WordByIdQuery : public WordsQuery
{
public:
    WordByIdQuery( Database* db, Lang src, QObject* parent = NULL )
        : WordsQuery( db, src, parent ) {}

protected: // from Query
    virtual QString buildSql() const;
    virtual int bind();
};

// ----------------------------------------------------------------------------

class WordsByPatternQuery : public WordsQuery
{
public:
    WordsByPatternQuery( Database* db, Lang src, QObject* parent = NULL )
        : WordsQuery( db, src, parent ) {}

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
    WordUpdateQuery( Database* db, Lang src, QObject* parent = NULL )
        : WordsQuery( db, src, parent ) {}

    virtual int execute();

protected: // from Query
    virtual QString buildSql() const;
    virtual int bind();
};

// ----------------------------------------------------------------------------

class WordInsertQuery : public WordUpdateQuery
{
public:
    WordInsertQuery( Database* db, Lang src, QObject* parent = NULL )
        : WordsInsertQuery( db, src, parent ) {}

    virtual int execute();

protected: // from Query
    virtual QString buildSql() const;
    virtual bool execute();
};

#endif // WORDQUERY_H
