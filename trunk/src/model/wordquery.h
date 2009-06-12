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
    const Record& record() const;

protected: // from Query
    virtual int bind();
    virtual void read();

    QString selectBaseSql() const;
    QString updateBaseSql() const;
    QString insertBaseSql() const;
    QString deleteBaseSql() const;

private:
    WordsQuery();
    Q_DISABLE_COPY( WordsQuery );

protected:
    Record m_record;
};

class WordByIdQuery : public WordsQuery
{
public:
    WordByIdQuery( sqlite* db, Lang src, QObject* parent = NULL );
    void setId( qint64 id );

protected: // from Query
    virtual int bind();

private:
    qint64 m_id;
};

class WordsByPatternQuery : public WordsQuery
{
public:
    WordsByPatternQuery( sqlite* db, Lang src, QObject* parent = NULL );
    void setPattern( const QString& pattern, TextMatch match );

protected: // from Query
    virtual int bind();

private:
    QString m_pattern;
    TextMatch m_match;
};

#endif // WORDQUERY_H
