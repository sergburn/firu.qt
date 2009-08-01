/*
 * AppUi.h
 *
 *  Created on: 28.03.2009
 *      Author: sergey
 */

#ifndef APPUI_H_
#define APPUI_H_

#include <QtCore/QString>
#include <QtCore/QRect>

class AppUiBase
{
public:
    virtual void SetAppTitle( QString title ) = 0;
    virtual QRect GetClientRect() const = 0;
    virtual QRect GetAppRect() const = 0;
};

class AppUi : public AppUiBase
{
public:
    AppUi();
    virtual ~AppUi();
    
    void SetAppTitle( QString title );
    QRect GetClientRect() const;
    QRect GetAppRect() const;

private:
    AppUiBase* m_impl;
};

#endif /* APPUI_H_ */
