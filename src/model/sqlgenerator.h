#ifndef SQLGENERATOR_H
#define SQLGENERATOR_H

class SqlGenerator
{
public:
    SqlGenerator( const QString& baseSql );

    void addCondition( const QString& condition );
    void addPrimaryKeyCondition();
    void addSorting( const QString& field, bool ascending = true );
    void addSet( const QString& expr );

    QString sql() const;

    static QString createPattern( const QString& text, TextMatch match );

    static QString selectBaseSql( const QString& table ) const;
    static QString countBaseSql( const QString& table ) const;
    static QString updateBaseSql( const QString& table ) const;
    static QString deleteBaseSql( const QString& table ) const;

private:
    QString sql;

    int m_conditions;
    int m_sets;
};

#endif // SQLGENERATOR_H
