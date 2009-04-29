#ifndef FIRUMAINWINDOW_H
#define FIRUMAINWINDOW_H

#include <QtGui/QMainWindow>
#include "ui_firumainwindow.h"
#include "AppUi.h"
#include "data.h"

class FiruMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    FiruMainWindow( Data& data, QWidget *parent = 0 );
    ~FiruMainWindow();

public slots:
    void setInputWord( QString );
    void importDict( const QString& file );
    
signals:
    void inputWordChanged( QString );
    
protected:
//    virtual void changeEvent(QEvent *e);
//    virtual void keyPressEvent(QKeyEvent *keyEvent);
    virtual bool eventFilter(QObject *obj, QEvent *ev);
    
private:
    void updateList();
    
private slots:
    void on_actionOpenDict_triggered();
    void on_actionOpenTrainer_triggered();

private:
    Ui::FiruMainWindowClass m_ui;
    AppUi m_appUi;
    QString m_word;
    Data& m_data;
    QList<QListWidgetItem> m_listItems;
};

#endif // FIRUMAINWINDOW_H
