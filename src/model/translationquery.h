#ifndef TRANSLATIONQUERY_H
#define TRANSLATIONQUERY_H

#include <QSharedPointer>
#include "query.h"

/** Selects all translations */
class TranslationsQuery : public Query
{
    Q_OBJECT
public:
    SHARED_POINTER( TranslationsQuery );
    TranslationsQuery( Database* db, LangPair langs );

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
    virtual void read();

private:
    TranslationsQuery();
    Q_DISABLE_COPY( TranslationsQuery );

protected:
    Record m_record;
};

// ----------------------------------------------------------------------------

class TranslationByIdQuery : public TranslationsQuery
{
    Q_OBJECT
public:
    SHARED_POINTER( TranslationByIdQuery )
    TranslationByIdQuery( Database* db, LangPair langs ) : TranslationsQuery( db, langs ) {}

protected: // from Query
    virtual QString buildSql() const;
    virtual int bind();
};

// ----------------------------------------------------------------------------

class TranslationsBySidQuery : public TranslationsQuery
{
    Q_OBJECT
public:
    SHARED_POINTER( TranslationsBySidQuery )
    TranslationsBySidQuery( Database* db, LangPair langs ) : TranslationsQuery( db, langs ) {}

    void setSourceEntry( qint64 id );

protected: // from Query
    virtual QString buildSql() const;
    virtual int bind();

private:
    qint64 m_sid;
};

// ----------------------------------------------------------------------------

class TranslationsByRMarkQuery : public TranslationsQuery
{
    Q_OBJECT
public:
    SHARED_POINTER( TranslationsByRMarkQuery )
    TranslationsByRMarkQuery( Database* db, LangPair langs ) : TranslationsQuery( db, langs ) {}

    void setRequiredMark( Mark::MarkValue mark );

protected: // from Query
    virtual QString buildSql() const;
    virtual int bind();

private:
    int m_rmark;
};

// ----------------------------------------------------------------------------

class TranslationsByPatternQuery : public TranslationsQuery
{
    Q_OBJECT
public:
    SHARED_POINTER( TranslationsByPatternQuery )
    TranslationsByPatternQuery( Database* db, LangPair langs ) : TranslationsQuery( db, langs ) {}

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
    Q_OBJECT
public:
    SHARED_POINTER( TranslationUpdateQuery )
    TranslationUpdateQuery( Database* db, LangPair langs ) : TranslationsQuery( db, langs ) {}

protected: // from Query
    virtual QString buildSql() const;
    virtual int bind();
};

// ----------------------------------------------------------------------------

class TranslationInsertQuery : public TranslationUpdateQuery
{
    Q_OBJECT
public:
    SHARED_POINTER( TranslationInsertQuery )
    TranslationInsertQuery( Database* db, LangPair langs ) : TranslationUpdateQuery( db, langs ) {}

    virtual bool execute();

protected: // from Query
    virtual QString buildSql() const;
};

// ----------------------------------------------------------------------------

class UpdateMarksQuery : public TranslationsQuery
{
    Q_OBJECT
public:
    SHARED_POINTER( UpdateMarksQuery );

    UpdateMarksQuery( Database* db, LangPair langs );

protected: // from Query
    virtual QString buildSql() const;
    virtual int bind();
};

#endif // TRANSLATIONQUERY_H
