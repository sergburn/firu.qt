#ifndef ENTRYVIEWDIALOG_H
#define ENTRYVIEWDIALOG_H

#include <QtGui/QDialog>

#include "AppUi.h"

namespace Ui {
    class EntryViewDialog;
}

class EntryViewDialog : public QDialog {
    Q_OBJECT
    Q_DISABLE_COPY(EntryViewDialog)
public:
    explicit EntryViewDialog(QWidget *parent = 0);
    virtual ~EntryViewDialog();

public slots:
    int exec( const QString& entry, const QStringList& translations );

protected:
    virtual void changeEvent(QEvent *e);
    virtual void keyPressEvent( QKeyEvent *keyEvent );
    virtual void mousePressEvent( QMouseEvent *mouseEvent );

private:
    Ui::EntryViewDialog *m_ui;
    AppUi m_appUi;
};

#endif // ENTRYVIEWDIALOG_H
