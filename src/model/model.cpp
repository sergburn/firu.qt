#include "model.h"

// ----------------------------------------------------------------------------

quint64 WordKey::getStringKey( const QString& text )
{
    quint64 l[] = { 0, 0, 0, 0 };

    for ( int i = 0; i < 4; i++ )
    {
        if ( text.length() > i )
        {
            l[i] = text[i].toLower().unicode();
        }
    }

    quint64 hash = l[0] << 48 | l[1] << 32 | l[2]<< 16 | l[3];

    QString str;
    qDebug() << "Hash for text" << text << "is" << str.setNum( hash, 16 );
    return hash;
}

// ----------------------------------------------------------------------------

void WordKey::getPatternKeyLimits( const QString& text, quint64& min, quint64& max )
{
    min = max = getStringKey( text );
    if ( text.length() )
    {
        quint64 shift = 0x0001000000000000;
        for ( int i = 1; i < 4; i++ )
        {
            if ( text.length() > i )
            {
                shift >>= 16;
            }
        }
        max = min + shift;
    }
    QString smax;
    qDebug() << "Max limit for pattern" << text << "is" << smax.setNum( max, 16 );
}
