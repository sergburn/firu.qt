/*
 * AppUi.h
 *
 *  Created on: 28.03.2009
 *      Author: sergey
 */

#ifndef APPUI_H_
#define APPUI_H_

#include <eikenv.h>
#include <aknappui.h>
#include <QtCore/QString>
#include <QtCore/QRect>

class AppUi
{
public:
    AppUi();
    virtual ~AppUi();
    
    void SetAppTitle( QString title );
    QRect GetClientRect() const;
    
private:
    CEikonEnv* m_eikon;
    CAknAppUi* m_appui;
    CEikStatusPane* m_statusPane;
};

#endif /* APPUI_H_ */
