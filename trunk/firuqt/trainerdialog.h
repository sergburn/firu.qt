#ifndef TRAINERDIALOG_H
#define TRAINERDIALOG_H

#include <QtGui/QDialog>
#include "AppUi.h"

namespace Ui {
    class TrainerDialog;
}

class TrainerDialog : public QDialog {
    Q_OBJECT
    Q_DISABLE_COPY(TrainerDialog)
public:
    explicit TrainerDialog(QWidget *parent = 0);
    virtual ~TrainerDialog();

protected:
    virtual void changeEvent(QEvent *e);
    virtual void keyPressEvent(QKeyEvent *keyEvent);

private:
    void showAnswersList();
    void showKeypad();

private:
    Ui::TrainerDialog *m_ui;
    AppUi m_appUi;
};

#endif // TRAINERDIALOG_H
