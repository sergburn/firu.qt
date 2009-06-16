#ifndef TRANSLATIONQUERY_H
#define TRANSLATIONQUERY_H

#include <QSharedPointer>
#include "query.h"

/** Selects all translations */
class TranslationsQuery : public Query
{
public:
    SHARED_POINTER( TranslationsQuery );
    TranslationsQuery( Database* db, LangPair langs, QObject* parent = NULL );

    enum Sort
    {
        NONE, FMARK, RMARK
    };
    void setSort( Sort sort, bool ascending = true );

    struct Record
    {
        qint64 id;
        qint64 sid;
        QString text;
        int fmark;
        int rmark;
    };
    Record& record();
    const Record& record() const;

protected: // from Query
    virtual QString buildSql() const;
    virtual int bind();
    virtual void read();

private:
    TranslationsQuery();
    Q_DISABLE_COPY( TranslationsQuery );

protected:
    Record m_record;
    Sort m_sort;
};

// ----------------------------------------------------------------------------

class TranslationByIdQuery : public TranslationsQuery
{
public:
    SHARED_POINTER( TranslationByIdQuery )
    TranslationByIdQuery( Database* db, LangPair langs, QObject* parent = NULL );

protected: // from Query
    virtual QString buildSql() const;
    virtual int bind();
};

// ----------------------------------------------------------------------------

class TranslationsBySidQuery : public TranslationsQuery
{
public:
    SHARED_POINTER( TranslationsBySidQuery )
    TranslationsBySidQuery( Database* db, LangPair langs, QObject* parent = NULL );

    void setSourceEntry( qint64 id );

protected: // from Query
    virtual QString buildSql() const;
    virtual int bind();

private:
    qint64 m_sid;
};

// ----------------------------------------------------------------------------

class TranslationsByPatternQuery : public TranslationsQuery
{
public:
    SHARED_POINTER( TranslationsByPatternQuery )
    TranslationsByPatternQuery( Database* db, LangPair langs, QObject* parent = NULL )
        : TranslationsQuery( db, langs, parent ) {}

    void setPattern( const QString& pattern, TextMatch match );

protected: // from Query
    virtual QString buildSql() const;
    virtual int bind();

private:
    QString m_pattern;
    TextMatch m_match;
};

// ----------------------------------------------------------------------------

class TranslationUpdateQuery : public TranslationsQuery
{
public:
    SHARED_POINTER( TranslationUpdateQuery )
    TranslationUpdateQuery( Database* db, LangPair langs, QObject* parent = NULL );

protected: // from Query
    virtual QString buildSql() const;
    virtual int bind();
};

// ----------------------------------------------------------------------------

class TranslationInsertQuery : public TranslationUpdateQuery
{
public:
    SHARED_POINTER( TranslationInsertQuery )
    TranslationInsertQuery( Database* db, LangPair langs, QObject* parent = NULL );

    virtual bool execute();

protected: // from Query
    virtual QString buildSql() const;
};

// ----------------------------------------------------------------------------

class UpdateMarksQuery : public TranslationsQuery
{
public:
    SHARED_POINTER( UpdateMarksQuery );

    UpdateMarksQuery( Database* db, LangPair langs, QObject* parent = NULL );

    Mark::MarkValue m_fMarkValue;
    Mark::MarkValue m_rMarkValue;
    void resetMarks();

protected: // from Query
    virtual QString buildSql() const;
    virtual bool execute();
};

#endif // TRANSLATIONQUERY_H
