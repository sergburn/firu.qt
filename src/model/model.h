#ifndef MODEL_H
#define MODEL_H

#include <QLocale>

typedef QLocale::Language Lang;

enum TextMatch
{
    FullMatch,
    StartsWith,
    Contains,
    EndsWith
};

class Mark
{
public:
    enum MarkValue
    {
        Undefined   = -1,
        Unknown     = 0,
        ToLearn     = 1,
        Good        = 2,
        Better      = 3,
        Learned     = 4
    };

public:
    Mark() : m_value( Undefined ) {}

    MarkValue upgrade()
    {
        if ( m_value < Learned )
        {
            m_value = (MarkValue)((int)m_value+1);
        }
        return m_value;
    }
    MarkValue drop() { return m_value = ToLearn; }
    MarkValue clear() { return m_value = Unknown; }
    MarkValue reset() { return m_value = Undefined; }
    MarkValue init() { return m_value = ToLearn; }
    MarkValue operator()() const { return m_value; }

private:
    MarkValue m_value;
};

#endif // MODEL_H
