#ifndef TRANSLATIONQUERY_H
#define TRANSLATIONQUERY_H

#include <QSharedPointer>
#include "model.h"
#include "query.h"

/** Selects all translations */
class TranslationQuery : public Query
{
public:
    typedef QSharedPointer<TranslationQuery> Ptr;

    TranslationQuery( sqlite3* db, Lang src, Lang trg );

    // Filters
    /** Selects translations by ID */
    qint64 filterId;
    /** Selects translations by source entry ID */
    qint64 filterSid;
    /** Selects translations by fmark */
    int filterFmark;
    /** Selects translations by rmark */
    int filterRmark;

    void resetFilters();

protected: // from Query
    virtual int bind();

private:
    TranslationQuery();
    Q_DISABLE_COPY( TranslationQuery );
};

// ----------------------------------------------------------------------------

class SelectTranslationsQuery : public TranslationQuery
{
public:
    typedef QSharedPointer<SelectTranslationQuery> Ptr;

    SelectTranslationQuery( sqlite3* db, Lang src, Lang trg );

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
    const Record& record() const;

private:
    SelectTranslationsQuery();
    Q_DISABLE_COPY( SelectTranslationsQuery );

protected: // from Query
    virtual int prepare();
    virtual void read();
    virtual void doReset { resetFilters(); }

protected:
    Record m_record;
    Sort m_sort;
};

// ----------------------------------------------------------------------------

class UpdateMarkQuery : public TranslationQuery
{
public:
    typedef QSharedPointer<UpdateMarkQuery> Ptr;

    UpdateMarkQuery( sqlite3* db, Lang src, Lang trg );

    Mark::MarkValue m_fMarkValue;
    Mark::MarkValue m_rMarkValue;
    void resetMarks();

protected:
    virtual int prepare();
    virtual void read() {}
    virtual void doReset { TranslationQuery::reset(); resetMarks(); }

private:
    UpdateMarkQuery();
    Q_DISABLE_COPY( UpdateMarkQuery );
};

#endif // TRANSLATIONQUERY_H
