#include "dictionary.h"

// ----------------------------------------------------------------------------

Dictionary::Dictionary( Lang src, Lang trg )
    : m_vocab( src ), m_trans( src, trg )
{
}

// ----------------------------------------------------------------------------

Translations::Translations( Lang src, Lang trg )
    : m_srcLang( src ), m_trgLang( trg )
{
}

// ----------------------------------------------------------------------------

Vocabulary::Vocabulary( Lang src )
    : m_srcLang( src )
{
}

// ----------------------------------------------------------------------------
