#ifndef TRANSLATIONQUERY_H
#define TRANSLATIONQUERY_H

#include <QSharedPointer>
#include "query.h"

/** Selects all translations */
class TranslationQuery : public Query
{
public:
    typedef QSharedPointer<TranslationQuery> Ptr;

    TranslationQuery( sqlite3* db, Lang src, Lang trg );

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

    // Filters
    /** Selects translations by ID */
    qint64 filterId;
    /** Selects translations by source entry ID */
    qint64 filterSid;
    /** Selects translations by fmark */
    int filterFmark;
    /** Selects translations by rmark */
    int filterRmark;

protected: // from Query
    virtual int bind();
    virtual int prepare();
    virtual void read();

private:
    TranslationQuery();
    Q_DISABLE_COPY( TranslationQuery );

protected:
    Record m_record;
    Sort m_sort;
};

#endif // TRANSLATIONQUERY_H