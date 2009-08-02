#ifndef TRAINERDIALOG_H
#define TRAINERDIALOG_H

#include <QtGui/QDialog>
#include <QtGui/QKeyEvent>
#include <QtGui/QMouseEvent>
#include <QtGui/QLabel>
#include <QtGui/QGraphicsScene>
#include <QtGui/QGraphicsRectItem>
#include <QRect>

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

public slots:
    void showHints();

signals:
    void testFinished();
    void testCanceled();

protected:
    virtual void changeEvent( QEvent *e );
    virtual void keyPressEvent( QKeyEvent *keyEvent );
    virtual void mousePressEvent( QMouseEvent *mouseEvent );
    virtual void resizeEvent(QResizeEvent *e);
    virtual void moveEvent( QMoveEvent *e ); 
    
private:
    void setKeypadMode();
    void checkNextLetter( QString letter );
    void showNextLetters();
    void checkAnswer( QString answer );

    void leftCommand();
    void rightCommand();
    void drawMark( Mark::MarkValue mark );

private slots:
    void showResult();
    void updateMark();

private:
    Ui::TrainerDialog *m_ui;
    AppUi m_appUi;
    QList<QLabel*> m_keyLabels;

    ReverseTest::Ptr m_test;
    QStringList m_keyGroups;
    QString m_answerText;
};

#endif // TRAINERDIALOG_H
