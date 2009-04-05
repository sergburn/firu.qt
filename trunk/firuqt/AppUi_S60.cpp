/*
 * AppUi.cpp
 *
 *  Created on: 28.03.2009
 *      Author: sergey
 */

#include "AppUi.h"

#include <eikenv.h>
#include <aknappui.h>
#include <akntitle.h>

class AppUiS60 : public AppUi
{
    AppUiS60();
    virtual ~AppUiS60();

    virtual void SetAppTitle( QString title );
    virtual QRect GetClientRect() const;

private:
    CEikonEnv* m_eikon;
    CAknAppUi* m_appui;
    CEikStatusPane* m_statusPane;
};

// --------------------------------------------------------

AppUiS60::AppUiS60()
{
    m_eikon = CEikonEnv::Static();
    if ( m_eikon )
    {
        m_appui = (CAknAppUi*) m_eikon->AppUi();
        if ( m_appui )
        {
            m_statusPane = m_appui->StatusPane();
        }
    }
}

// --------------------------------------------------------

void AppUiS60::SetAppTitle( QString title )
{
    TPtrC16 textPtr( reinterpret_cast<const TUint16*>( title.utf16() ) );

    if ( m_statusPane )
    {
        TUid titlePaneUid = TUid::Uid( EEikStatusPaneUidTitle );
        CEikStatusPaneBase::TPaneCapabilities subPaneTitle =
            m_statusPane->PaneCapabilities( titlePaneUid );
        if ( subPaneTitle.IsPresent() && subPaneTitle.IsAppOwned() )
        {
            CAknTitlePane* title = static_cast< CAknTitlePane*>(
                m_statusPane->ControlL( titlePaneUid ) );
            title->SetTextL( textPtr );
            CleanupStack::PopAndDestroy(); // reader internal state
        }
    }
}

QRect AppUiS60::GetClientRect() const
{
    QRect rect;
    if ( m_appui )
    {
        TRect r = m_appui->ClientRect();
        rect.setCoords( r.iTl.iX, r.iTl.iY, r.iBr.iX, r.iBr.iY );
    }
    return rect;
}

// --------------------------------------------------------

AppUi::~AppUi()
{
    // TODO Auto-generated destructor stub
}
