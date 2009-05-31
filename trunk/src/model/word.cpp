#include "word.h"
#include "translation.h"

// ----------------------------------------------------------------------------

Word::Word()
    : m_id( 0 )
{
}

// ----------------------------------------------------------------------------

Word::Word( qint64 id, const QString& text )
    : m_id( id ), m_text( text )
{
}

// ----------------------------------------------------------------------------

QList<Translation*> Word::translations( Lang trg )
{
    if ( m_translations.count() == 0 )
    {
        // try to load translations
        m_translations = Translation::findBySourceEntry( m_id, m_lang, trg );
    }
    return m_translations;
}
