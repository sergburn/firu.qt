#include "sqlgenerator.h"

// ----------------------------------------------------------------------------

SqlGenerator::SqlGenerator( const QString& baseSql )
    : m_sql( baseSql ), 
    m_conditions( 0 ), 
    m_sets( 0 ), 
    m_sort( false ),
    m_limit( false )
{
}

// ----------------------------------------------------------------------------

QString SqlGenerator::sql() const
{
    return m_sql;
}

// ----------------------------------------------------------------------------

void SqlGenerator::addCondition( const QString& condition )
{
    if ( m_conditions == 0 ) m_sql.append( " WHERE " );
    if ( m_conditions > 0 ) m_sql.append( " AND " );

    m_sql.append( condition );
    m_conditions++;
}

// ----------------------------------------------------------------------------

void SqlGenerator::addPrimaryKeyCondition()
{
    addCondition( "id = :id" );
}

// ----------------------------------------------------------------------------

void SqlGenerator::addSet( const QString& expr )
{
    if ( m_sets > 0 ) m_sql.append( ", " );
    m_sql.append( expr );
    m_sets++;
}

// ----------------------------------------------------------------------------

void SqlGenerator::addSorting( const QString& field, bool ascending )
{
    if ( !m_sort )
    {
        QString sort( " ORDER BY %1" );
        if ( !ascending ) sort.append( " DESC" );
        m_sql.append( sort.arg( field ) );
        m_sort = true;
    }
}

// ----------------------------------------------------------------------------

void SqlGenerator::addLimit( long maxResults )
{
    if ( !m_limit )
    {
        QString limit( " LIMIT %1" );
        m_sql.append( limit.arg( maxResults ) );
    }
}

// ----------------------------------------------------------------------------

QString SqlGenerator::createPattern( const QString& text, TextMatch match )
{
    QString pattern( text );
    switch ( match )
    {
        case StartsWith:
            pattern.append('%');
            break;
        case Contains:
            pattern.prepend('%').append('%');
            break;
        case EndsWith:
            pattern.prepend('%');
            break;
        case FullMatch:
        default:
            break;
    }
    return pattern;
}

// ----------------------------------------------------------------------------

QString SqlGenerator::selectBaseSql( const QString& table )
{
    return QString( "SELECT * FROM %1" ).arg( table );
}

// ----------------------------------------------------------------------------

QString SqlGenerator::countBaseSql( const QString& table )
{
    return QString( "SELECT count(*) FROM %1" ).arg( table );
}

// ----------------------------------------------------------------------------

QString SqlGenerator::updateBaseSql( const QString& table )
{
    return QString( "UPDATE %1 SET " ).arg( table );
}

// ----------------------------------------------------------------------------

QString SqlGenerator::deleteBaseSql( const QString& table )
{
    return QString( "DELETE FROM %1" ).arg( table );
}
