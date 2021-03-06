#ifndef MODEL_H
#define MODEL_H

#include <QLocale>
#include <QPair>
#include <QDebug>

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

enum TestResult
{
    Incomplete,
    Passed,             // adds 1 to current rate
    PassedWithHints,    // doesn't change rate if 1 or 2, demotes rate 3 to 2
    Failed              // sets current rate 1.
};

class Mark
{
public:
    enum MarkValue
    {
        Undefined       = -1,
        Unknown         = 0,
        ToLearn         = 1,
        WithHints       = 2,
        AlmostLearned   = 3,
        Learned         = 4
    };

public:
    Mark() : m_value( Undefined ) {}
    explicit Mark( MarkValue val ) : m_value( val ) {}
    MarkValue operator()() const { return m_value; }
//    Mark& operator=( MarkValue value ) { m_value = value; return *this; }
    operator MarkValue() { return m_value; }

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

    MarkValue updateToTestResult( TestResult result )
    {
        MarkValue original = m_value;
        switch ( result )
        {
            case Passed:
                if ( m_value == ToLearn )
                    m_value = AlmostLearned;
                else
                    upgrade();
                break;

            case PassedWithHints:
                m_value = WithHints;
                break;

            case Failed:
                m_value = ToLearn;
                break;

            default:
                qDebug() << "Unexpected test result" << result << "in Mark::updateToTestResult";
                break;
        }
        qDebug() << "Mark changed from" << original << "to" << m_value;
        return m_value;
    }
    
    static QString toString( MarkValue mark )
    {
        switch ( mark )
        {
            case Unknown:
                return "New";
            case ToLearn:
                return "Need to learn";
            case WithHints:
                return "With hints";
            case AlmostLearned:
                return "Well known";
            case Learned:
                return "Learned";
            case Undefined:
            default:
                return "<none>";
        }
    }
    
    QString toString() const
    {
        return toString( m_value );
    }

private:
    MarkValue m_value;
};

class WordKey
{
public:
    static quint64 getStringKey( const QString& text );
    static void getPatternKeyLimits( const QString& text, quint64& min, quint64& max );
};

#endif // MODEL_H
