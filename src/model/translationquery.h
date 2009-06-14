#ifndef TRANSLATIONQUERY_H
#define TRANSLATIONQUERY_H

#include <QSharedPointer>
#include "model.h"
#include "query.h"

/** Selects all translations */
class TranslationsQuery : public Query
{
public:
    typedef QSharedPointer<TranslationsQuery> Ptr;

    TranslationsQuery( sqlite3* db, Lang src, Lang trg, QObject* parent = NULL );

    enum Sort
    {
        NONE, FMARK, RMARK
    };
    void setSort( Sort sort, bool ascending = true );

    class Record
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
    TranslationByIdQuery( sqlite* db, Lang src, Lang trg, QObject* parent = NULL );

protected: // from Query
    virtual QString buildSql() const;
    virtual int bind();
};

// ----------------------------------------------------------------------------

class TranslationsBySidQuery : public TranslationsQuery
{
public:
    TranslationsBySidQuery( sqlite* db, Lang src, Lang trg, QObject* parent = NULL );

    void setSourceEntry( qint64 id );

protected: // from Query
    virtual QString buildSql() const;
    virtual int bind();

private:
    qint64 m_sid;
};

// ----------------------------------------------------------------------------

class TranslationUpdateQuery : public TranslationsQuery
{
public:
    TranslationUpdateQuery( sqlite* db, Lang src, Lang trg, QObject* parent = NULL );

    virtual int execute();

protected: // from Query
    virtual QString buildSql() const;
    virtual int bind();
};

// ----------------------------------------------------------------------------

class TranslationInsertQuery : public TranslationsUpdateQuery
{
public:
    TranslationInsertQuery( sqlite* db, Lang src, Lang trg, QObject* parent = NULL );

    virtual int execute();

protected: // from Query
    virtual QString buildSql() const;
    virtual bool execute();
};

// ----------------------------------------------------------------------------

class UpdateMarksQuery : public TranslationsQuery
{
public:
    typedef QSharedPointer<UpdateMarkQuery> Ptr;

    UpdateMarksQuery( sqlite3* db, Lang src, Lang trg, QObject* parent = NULL );

    Mark::MarkValue m_fMarkValue;
    Mark::MarkValue m_rMarkValue;
    void resetMarks();

protected: // from Query
    virtual QString buildSql() const;
    virtual bool execute();
};

#endif // TRANSLATIONQUERY_H
