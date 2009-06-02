#ifndef WORDQUERY_H
#define WORDQUERY_H

class WordQuery : public Query
{
public:
    typedef QSharedPointer<WordQuery> Ptr;

    WordQuery( Lang src );

    class Record
    {
        qint64 id;
        QString text;
    };
    const Record& record() const;

    // Filters
    /** Selects words by ID */
    qint64 filterId;
    /** Selects words by Pattern */
    QString filterPattern;

protected: // from Query
    virtual int bind();
    virtual int prepare();
    virtual void read();

private:
    WordQuery();
    Q_DISABLE_COPY( WordQuery );

protected:
    Record m_record;
};

#endif // WORDQUERY_H
