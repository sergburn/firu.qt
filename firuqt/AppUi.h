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

class AppUi
{
public:
    AppUi();
    virtual ~AppUi();
    
    void SetAppTitle( QString title );
    QRect GetClientRect() const;

private:
    AppUi* m_impl;
};

#endif /* APPUI_H_ */
