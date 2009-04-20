/********************************************************************************
** Form generated from reading ui file 'firumainwindow.ui'
**
** Created: Fri 10. Apr 22:29:26 2009
**      by: Qt User Interface Compiler version 4.5.0-garden
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
    QLineEdit *editInput;
    QListWidget *listSources;
    QLabel *laTranslation;
    QMenuBar *menubar;

    void setupUi(QMainWindow *FiruMainWindowClass)
    {
        if (FiruMainWindowClass->objectName().isEmpty())
            FiruMainWindowClass->setObjectName(QString::fromUtf8("FiruMainWindowClass"));
        FiruMainWindowClass->resize(274, 341);
        FiruMainWindowClass->setStyleSheet(QString::fromUtf8("\n"
"font: 75 10pt \"MS Shell Dlg 2\";background-color: rgb(255, 255, 255);\n"
"border-color: rgb(0, 0, 127);\n"
"color: rgb(0, 0, 0);"));
        actionOpenDict = new QAction(FiruMainWindowClass);
        actionOpenDict->setObjectName(QString::fromUtf8("actionOpenDict"));
        actionOpenTrainer = new QAction(FiruMainWindowClass);
        actionOpenTrainer->setObjectName(QString::fromUtf8("actionOpenTrainer"));
        centralwidget = new QWidget(FiruMainWindowClass);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        editInput = new QLineEdit(centralwidget);
        editInput->setObjectName(QString::fromUtf8("editInput"));
        QFont font;
        font.setFamily(QString::fromUtf8("MS Shell Dlg 2"));
        font.setPointSize(10);
        font.setBold(false);
        font.setItalic(false);
        font.setWeight(9);
        editInput->setFont(font);
        editInput->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);\n"
"background-color: rgb(255, 255, 127);"));

        verticalLayout->addWidget(editInput);

        listSources = new QListWidget(centralwidget);
        new QListWidgetItem(listSources);
        new QListWidgetItem(listSources);
        new QListWidgetItem(listSources);
        new QListWidgetItem(listSources);
        listSources->setObjectName(QString::fromUtf8("listSources"));

        verticalLayout->addWidget(listSources);

        laTranslation = new QLabel(centralwidget);
        laTranslation->setObjectName(QString::fromUtf8("laTranslation"));
        laTranslation->setFont(font);
        laTranslation->setFrameShape(QFrame::StyledPanel);
        laTranslation->setWordWrap(true);

        verticalLayout->addWidget(laTranslation);

        FiruMainWindowClass->setCentralWidget(centralwidget);
        menubar = new QMenuBar(FiruMainWindowClass);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 274, 24));
        menubar->setStyleSheet(QString::fromUtf8("\n"
"border-color: rgb(0, 0, 0);color: rgb(0, 0, 0);\n"
"background-color: rgb(255, 255, 255);\n"
"selection-background-color: rgb(170, 255, 255);\n"
"selection-color: rgb(0, 0, 127);"));
        FiruMainWindowClass->setMenuBar(menubar);
        QWidget::setTabOrder(editInput, listSources);

        retranslateUi(FiruMainWindowClass);
        QObject::connect(FiruMainWindowClass, SIGNAL(inputWordChanged(QString)), laTranslation, SLOT(setText(QString)));
        QObject::connect(editInput, SIGNAL(textChanged(QString)), FiruMainWindowClass, SLOT(setInputWord(QString)));
        QObject::connect(listSources, SIGNAL(currentTextChanged(QString)), FiruMainWindowClass, SLOT(setInputWord(QString)));

        QMetaObject::connectSlotsByName(FiruMainWindowClass);
    } // setupUi

    void retranslateUi(QMainWindow *FiruMainWindowClass)
    {
        FiruMainWindowClass->setWindowTitle(QApplication::translate("FiruMainWindowClass", "Firu", 0, QApplication::UnicodeUTF8));
        actionOpenDict->setText(QApplication::translate("FiruMainWindowClass", "OpenDict", 0, QApplication::UnicodeUTF8));
        actionOpenTrainer->setText(QApplication::translate("FiruMainWindowClass", "OpenTrainer", 0, QApplication::UnicodeUTF8));

        const bool __sortingEnabled = listSources->isSortingEnabled();
        listSources->setSortingEnabled(false);
        QListWidgetItem *___qlistwidgetitem = listSources->item(0);
        ___qlistwidgetitem->setText(QApplication::translate("FiruMainWindowClass", "aamu", 0, QApplication::UnicodeUTF8));
        QListWidgetItem *___qlistwidgetitem1 = listSources->item(1);
        ___qlistwidgetitem1->setText(QApplication::translate("FiruMainWindowClass", "kana", 0, QApplication::UnicodeUTF8));
        QListWidgetItem *___qlistwidgetitem2 = listSources->item(2);
        ___qlistwidgetitem2->setText(QApplication::translate("FiruMainWindowClass", "liha", 0, QApplication::UnicodeUTF8));
        QListWidgetItem *___qlistwidgetitem3 = listSources->item(3);
        ___qlistwidgetitem3->setText(QApplication::translate("FiruMainWindowClass", "uimahalli", 0, QApplication::UnicodeUTF8));
        listSources->setSortingEnabled(__sortingEnabled);

        laTranslation->setText(QApplication::translate("FiruMainWindowClass", "TextLabel", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(FiruMainWindowClass);
    } // retranslateUi

};

namespace Ui {
    class FiruMainWindowClass: public Ui_FiruMainWindowClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FIRUMAINWINDOW_H
