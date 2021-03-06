#ifndef WORDQUERY_H
#define WORDQUERY_H

#include <QSharedPointer>
#include "query.h"

class WordsQuery : public Query
{
    Q_OBJECT
public:
    SHARED_POINTER( WordsQuery )

    WordsQuery( Database* db, LangPair langs );

    struct Record
    {
        qint64 id;
        QString text;
    };
    Record& record();
    const Record& record() const;

protected: // from Query
    virtual QString buildSql() const;
    virtual void read();

private:
    WordsQuery();
    Q_DISABLE_COPY( WordsQuery )

protected:
    Record m_record;
};

// ----------------------------------------------------------------------------

class WordsCountQuery : public WordsQuery
{
    Q_OBJECT
public:
    SHARED_POINTER( WordsCountQuery )
    WordsCountQuery( Database* db, LangPair langs ) : WordsQuery( db, langs ) {}

protected: // from Query
    virtual QString buildSql() const;
};

// ----------------------------------------------------------------------------

class WordByIdQuery : public WordsQuery
{
    Q_OBJECT
public:
    SHARED_POINTER( WordByIdQuery )
    WordByIdQuery( Database* db, LangPair langs ) : WordsQuery( db, langs ) {}

protected: // from Query
    virtual QString buildSql() const;
    virtual int bind();
};

// ----------------------------------------------------------------------------

class WordsByPatternQuery : public WordsQuery
{
    Q_OBJECT
public:
    SHARED_POINTER( WordsByPatternQuery )
    WordsByPatternQuery( Database* db, LangPair langs ) : WordsQuery( db, langs ) {}

    void setPattern( const QString& pattern, TextMatch match );

protected: // from Query
    virtual QString buildSql() const;
    virtual int bind();

private:
    QString m_pattern;
    TextMatch m_match;
};

// ----------------------------------------------------------------------------

class WordsByPatternCountQuery : public WordsQuery
{
    Q_OBJECT
public:
    SHARED_POINTER( WordsByPatternCountQuery )
    WordsByPatternCountQuery( Database* db, LangPair langs ) : WordsQuery( db, langs ) {}

    void setPattern( const QString& pattern );

protected: // from Query
    virtual QString buildSql() const;
    virtual int bind();
    virtual void read();

private:
    QString m_pattern;
};

// ----------------------------------------------------------------------------

class WordUpdateQuery : public WordsQuery
{
    Q_OBJECT
public:
    SHARED_POINTER( WordUpdateQuery )
    WordUpdateQuery( Database* db, LangPair langs ) : WordsQuery( db, langs ) {}

protected: // from Query
    virtual QString buildSql() const;
    virtual int bind();
};

// ----------------------------------------------------------------------------

class WordInsertQuery : public WordUpdateQuery
{
    Q_OBJECT
public:
    SHARED_POINTER( WordInsertQuery )
    WordInsertQuery( Database* db, LangPair langs ) : WordUpdateQuery( db, langs ) {}

    virtual bool execute();

protected: // from Query
    virtual QString buildSql() const;
};

#endif // WORDQUERY_H
