/*
 * AppUi.cpp
 *
 *  Created on: 28.03.2009
 *      Author: sergey
 */

#include "AppUi.h"
#include <QApplication>
#include <QDesktopWidget>

#ifdef __SYMBIAN32__
#include "AppUi_S60.h"
#endif

// --------------------------------------------------------

AppUi::AppUi()
{
#ifdef __SYMBIAN32__
    m_impl = new AppUiS60();
#else
    m_impl = NULL;
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
    {
        QDesktopWidget* desk = QApplication::desktop();
        QRect client = desk->availableGeometry();
        QRect qvga( 0, 0, 240, 320 );
        if ( client.width() >= qvga.width() && client.height() >= qvga.height() )
        {
            qvga.moveCenter( client.center() );
            return qvga;
        }
        else
        {
            return client;
        }
    }
}

QRect AppUi::GetAppRect() const
{
    if ( m_impl )
        return m_impl->GetAppRect();
    else
        return QRect();
}

// --------------------------------------------------------

AppUi::~AppUi()
{
    // TODO Auto-generated destructor stub
}
