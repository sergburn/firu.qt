#ifndef FIRUMAINWINDOW_H
#define FIRUMAINWINDOW_H

#include <QtGui/QMainWindow>
#include "ui_firumainwindow.h"
#include "AppUi.h"

class FiruMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    FiruMainWindow(QWidget *parent = 0);
    ~FiruMainWindow();

public Q_SLOTS:
    void setInputWord( QString );
    
Q_SIGNALS:
    void inputWordChanged( QString );
    
private:
    Ui::FiruMainWindowClass m_ui;
    AppUi m_appUi;
    QString m_word;
};

#endif // FIRUMAINWINDOW_H
