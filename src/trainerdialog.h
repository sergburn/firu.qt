#ifndef TRAINERDIALOG_H
#define TRAINERDIALOG_H

#include <QtGui/QDialog>
#include <QtGui/QKeyEvent>
#include <QtGui/QMouseEvent>
#include <QtGui/QLabel>

#include "AppUi.h"
#include "model/reversetest.h"

namespace Ui {
    class TrainerDialog;
}

class TrainerDialog : public QDialog {
    Q_OBJECT
    Q_DISABLE_COPY(TrainerDialog)
public:
    explicit TrainerDialog(QWidget *parent = 0);
    virtual ~TrainerDialog();

    void showTest( ReverseTest::Ptr test );

protected:
    virtual void changeEvent(QEvent *e);
    virtual void keyPressEvent(QKeyEvent *keyEvent);
    virtual void mousePressEvent( QMouseEvent *mouseEvent );

private:
    void showAnswersList();
    void showKeypad();
    void checkNextLetter( QString letter );
    void showNextLetters();
    void hideNextLetters();

private:
    Ui::TrainerDialog *m_ui;
    AppUi m_appUi;
    QList<QLabel*> m_keyLabels;

    ReverseTest::Ptr m_test;
    QStringList m_keyGroups;
    QString m_answerText;
};

#endif // TRAINERDIALOG_H
