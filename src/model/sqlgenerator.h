#ifndef SQLGENERATOR_H
#define SQLGENERATOR_H

#include <QString>
#include "model.h"

class SqlGenerator
{
public:
    SqlGenerator( const QString& baseSql );

    void addCondition( const QString& condition );
    void addPrimaryKeyCondition();
    void addSorting( const QString& field, bool ascending = true );
    void addSet( const QString& expr );
    void addLimit( long maxResults );
    
    QString sql() const;

    static QString createPattern( const QString& text, TextMatch match );

    static QString selectBaseSql( const QString& table );
    static QString countBaseSql( const QString& table );
    static QString updateBaseSql( const QString& table );
    static QString deleteBaseSql( const QString& table );

private:
    QString m_sql;

    int m_conditions;
    int m_sets;
    bool m_sort;
    bool m_limit;
};

#endif // SQLGENERATOR_H
