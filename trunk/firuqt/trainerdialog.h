#ifndef TRAINERDIALOG_H
#define TRAINERDIALOG_H

#include <QtGui/QDialog>

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

private:
    Ui::TrainerDialog *m_ui;
};

#endif // TRAINERDIALOG_H
