#ifndef MODEL_H
#define MODEL_H

#include <QLocale>
#include <QPair>

#define SHARED_POINTER(_class) typedef QSharedPointer<_class> Ptr;

typedef QLocale::Language Lang;
typedef QPair<Lang, Lang> LangPair;

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
    explicit Mark( MarkValue val ) : m_value( val ) {}
    MarkValue operator()() const { return m_value; }
//    Mark& operator=( MarkValue value ) { m_value = value; return *this; }

    MarkValue reset() { return m_value = Undefined; }
    MarkValue clear() { return m_value = Unknown; }
    MarkValue restart() { return m_value = ToLearn; }

    MarkValue upgrade()
    {
        if ( m_value < Learned )
        {
            m_value = (MarkValue)((int)m_value+1);
        }
        return m_value;
    }

    MarkValue downgrade()
    {
        if ( m_value > ToLearn )
        {
            m_value = (MarkValue)((int)m_value-1);
        }
        return m_value;
    }

private:
    MarkValue m_value;
};

#endif // MODEL_H
