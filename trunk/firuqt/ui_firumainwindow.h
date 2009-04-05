/********************************************************************************
** Form generated from reading ui file 'firumainwindow.ui'
**
** Created: Sun Apr 5 21:06:05 2009
**      by: Qt User Interface Compiler version 4.5.0
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_FIRUMAINWINDOW_H
#define UI_FIRUMAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QListWidget>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QPlainTextEdit>
#include <QtGui/QTextEdit>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FiruMainWindowClass
{
public:
    QAction *actionOpenDict;
    QAction *actionOpenTrainer;
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QLineEdit *lineEdit;
    QTextEdit *textEdit;
    QPlainTextEdit *plainTextEdit;
    QListWidget *listWidget;
    QLabel *label;
    QMenuBar *menubar;

    void setupUi(QMainWindow *FiruMainWindowClass)
    {
        if (FiruMainWindowClass->objectName().isEmpty())
            FiruMainWindowClass->setObjectName(QString::fromUtf8("FiruMainWindowClass"));
        FiruMainWindowClass->resize(274, 341);
        actionOpenDict = new QAction(FiruMainWindowClass);
        actionOpenDict->setObjectName(QString::fromUtf8("actionOpenDict"));
        actionOpenTrainer = new QAction(FiruMainWindowClass);
        actionOpenTrainer->setObjectName(QString::fromUtf8("actionOpenTrainer"));
        centralwidget = new QWidget(FiruMainWindowClass);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        lineEdit = new QLineEdit(centralwidget);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
        QFont font;
        font.setBold(true);
        font.setWeight(75);
        lineEdit->setFont(font);
        lineEdit->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);\n"
"background-color: rgb(255, 255, 127);"));

        verticalLayout->addWidget(lineEdit);

        textEdit = new QTextEdit(centralwidget);
        textEdit->setObjectName(QString::fromUtf8("textEdit"));
        textEdit->setStyleSheet(QString::fromUtf8("background-color: rgb(85, 255, 127);"));

        verticalLayout->addWidget(textEdit);

        plainTextEdit = new QPlainTextEdit(centralwidget);
        plainTextEdit->setObjectName(QString::fromUtf8("plainTextEdit"));
        plainTextEdit->setStyleSheet(QString::fromUtf8("background-color: rgb(170, 255, 255);"));

        verticalLayout->addWidget(plainTextEdit);

        listWidget = new QListWidget(centralwidget);
        new QListWidgetItem(listWidget);
        new QListWidgetItem(listWidget);
        new QListWidgetItem(listWidget);
        listWidget->setObjectName(QString::fromUtf8("listWidget"));

        verticalLayout->addWidget(listWidget);

        label = new QLabel(centralwidget);
        label->setObjectName(QString::fromUtf8("label"));
        QFont font1;
        font1.setPointSize(10);
        font1.setBold(true);
        font1.setWeight(75);
        label->setFont(font1);
        label->setFrameShape(QFrame::StyledPanel);
        label->setFrameShadow(QFrame::Plain);
        label->setWordWrap(true);

        verticalLayout->addWidget(label);

        FiruMainWindowClass->setCentralWidget(centralwidget);
        menubar = new QMenuBar(FiruMainWindowClass);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 274, 25));
        FiruMainWindowClass->setMenuBar(menubar);
        QWidget::setTabOrder(lineEdit, listWidget);

        retranslateUi(FiruMainWindowClass);
        QObject::connect(FiruMainWindowClass, SIGNAL(inputWordChanged(QString)), label, SLOT(setText(QString)));
        QObject::connect(lineEdit, SIGNAL(textChanged(QString)), FiruMainWindowClass, SLOT(setInputWord(QString)));
        QObject::connect(listWidget, SIGNAL(currentTextChanged(QString)), FiruMainWindowClass, SLOT(setInputWord(QString)));

        QMetaObject::connectSlotsByName(FiruMainWindowClass);
    } // setupUi

    void retranslateUi(QMainWindow *FiruMainWindowClass)
    {
        FiruMainWindowClass->setWindowTitle(QApplication::translate("FiruMainWindowClass", "Firu", 0, QApplication::UnicodeUTF8));
        actionOpenDict->setText(QApplication::translate("FiruMainWindowClass", "OpenDict", 0, QApplication::UnicodeUTF8));
        actionOpenTrainer->setText(QApplication::translate("FiruMainWindowClass", "OpenTrainer", 0, QApplication::UnicodeUTF8));

        const bool __sortingEnabled = listWidget->isSortingEnabled();
        listWidget->setSortingEnabled(false);
        QListWidgetItem *___qlistwidgetitem = listWidget->item(0);
        ___qlistwidgetitem->setText(QApplication::translate("FiruMainWindowClass", "Dictionary", 0, QApplication::UnicodeUTF8));
        QListWidgetItem *___qlistwidgetitem1 = listWidget->item(1);
        ___qlistwidgetitem1->setText(QApplication::translate("FiruMainWindowClass", "Trainer", 0, QApplication::UnicodeUTF8));
        QListWidgetItem *___qlistwidgetitem2 = listWidget->item(2);
        ___qlistwidgetitem2->setText(QApplication::translate("FiruMainWindowClass", "Advanced Trainer", 0, QApplication::UnicodeUTF8));
        listWidget->setSortingEnabled(__sortingEnabled);

        label->setText(QApplication::translate("FiruMainWindowClass", "TextLabel", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(FiruMainWindowClass);
    } // retranslateUi

};

namespace Ui {
    class FiruMainWindowClass: public Ui_FiruMainWindowClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FIRUMAINWINDOW_H
