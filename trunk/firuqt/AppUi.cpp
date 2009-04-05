/*
 * AppUi.cpp
 *
 *  Created on: 28.03.2009
 *      Author: sergey
 */

#include "AppUi.h"

#ifdef __SYMBIAN32__
#include "AppUi_S60.h"
#endif

// --------------------------------------------------------

AppUi::AppUi()
{
#ifdef __SYMBIAN32__
    m_impl = new AppUiS60();
#endif
}

// --------------------------------------------------------

void AppUi::SetAppTitle( QString title )
{
    if ( m_impl ) m_impl->SetAppTitle( title );
}

QRect AppUi::GetClientRect() const
{
    if ( m_impl )
        return m_impl->GetClientRect();
    else
        return QRect();
}

// --------------------------------------------------------

AppUi::~AppUi()
{
    // TODO Auto-generated destructor stub
}
