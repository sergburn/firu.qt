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
    
protected:
//    virtual void changeEvent(QEvent *e);
//    virtual void keyPressEvent(QKeyEvent *keyEvent);
    virtual bool eventFilter(QObject *obj, QEvent *ev);
    
private:
    Ui::FiruMainWindowClass m_ui;
    AppUi m_appUi;
    QString m_word;

private slots:
    void on_actionOpenDict_triggered();
    void on_actionOpenTrainer_triggered();
};

#endif // FIRUMAINWINDOW_H
