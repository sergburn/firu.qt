#ifndef FIRUMAINWINDOW_H
#define FIRUMAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QList>
#include <QListWidgetItem>
#include <QString>

#include "ui_firumainwindow.h"
#include "AppUi.h"
#include "model/dictionary.h"

class FiruMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    FiruMainWindow( QWidget *parent = 0 );
    ~FiruMainWindow();

public slots:
    void importDict( const QString& file );
    bool setDirection( Lang src, Lang trg, bool reverse = false );
    
protected:
//    virtual void changeEvent(QEvent *e);
//    virtual void keyPressEvent(QKeyEvent *keyEvent);
    virtual bool eventFilter( QObject *obj, QEvent *ev );
//    virtual void timerEvent( QTimerEvent* event );
    
private:
    void updateDirectionLabels();
    void scheduleListUpdate();
    void fillList( const Word::List& words );
    void updateList();
    
private slots:
    void loadList();
    void on_actionSearch_reverse_toggled( bool );
    void on_actionOpenDict_triggered();
    void on_actionOpenTrainer_triggered();
    void onPatternChanged( const QString& );
    void showTranslation( QListWidgetItem* );

private:
    Ui::FiruMainWindowClass m_ui;
    AppUi m_appUi;
    QString m_pattern;
    Dictionary::Ptr m_dictionary;
    bool m_reverse;
    Word::List m_words;
    QList<QListWidgetItem> m_listItems;
    QTimer m_loadTimer;
};

#endif // FIRUMAINWINDOW_H
