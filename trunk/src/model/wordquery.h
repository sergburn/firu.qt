#ifndef WORDQUERY_H
#define WORDQUERY_H

#include <QSharedPointer>
#include "query.h"

class WordsQuery : public Query
{
public:
    SHARED_POINTER( WordsQuery );

    WordsQuery( Database* db, Lang src, QObject* parent = NULL );

    struct Record
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
    SHARED_POINTER( WordsCountQuery )
    WordsCountQuery( Database* db, Lang src, QObject* parent = NULL )
        : WordsQuery( db, src, parent ) {}

protected: // from Query
    virtual QString buildSql() const;
};

// ----------------------------------------------------------------------------

class WordByIdQuery : public WordsQuery
{
public:
    SHARED_POINTER( WordByIdQuery )
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
    SHARED_POINTER( WordsByPatternQuery )
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
    SHARED_POINTER( WordUpdateQuery )
    WordUpdateQuery( Database* db, Lang src, QObject* parent = NULL )
        : WordsQuery( db, src, parent ) {}

protected: // from Query
    virtual QString buildSql() const;
    virtual int bind();
};

// ----------------------------------------------------------------------------

class WordInsertQuery : public WordUpdateQuery
{
public:
    SHARED_POINTER( WordInsertQuery )
    WordInsertQuery( Database* db, Lang src, QObject* parent = NULL )
        : WordUpdateQuery( db, src, parent ) {}

    virtual bool execute();

protected: // from Query
    virtual QString buildSql() const;
};

#endif // WORDQUERY_H
