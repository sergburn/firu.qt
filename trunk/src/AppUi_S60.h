/*
 * AppUi.h
 *
 *  Created on: 28.03.2009
 *      Author: sergey
 */

#ifndef APPUIS60_H_
#define APPUIS60_H_

#include <eikenv.h>
#include <aknappui.h>
#include <QtCore/QString>
#include <QtCore/QRect>
#include "AppUI.h"

class AppUiS60 : public AppUiBase
{
public:
    AppUiS60();
    virtual ~AppUiS60();
    
    void SetAppTitle( QString title );
    QRect GetClientRect() const;
    QRect GetAppRect() const;
    
private:
    CEikonEnv* m_eikon;
    CAknAppUi* m_appui;
    CEikStatusPane* m_statusPane;
};

#endif /* APPUIS60_H_ */
