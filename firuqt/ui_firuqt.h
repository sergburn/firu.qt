/********************************************************************************
** Form generated from reading ui file 'firuqt.ui'
**
** Created: Fri 10. Apr 21:55:34 2009
**      by: Qt User Interface Compiler version 4.5.0-garden
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_FIRUQT_H
#define UI_FIRUQT_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLineEdit>
#include <QtGui/QListView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_firuqtClass
{
public:
    QAction *actionGoNext;
    QAction *actionGoBack;
    QGridLayout *gridLayout;
    QListView *listView;
    QLineEdit *lineEdit;

    void setupUi(QWidget *firuqtClass)
    {
        if (firuqtClass->objectName().isEmpty())
            firuqtClass->setObjectName(QString::fromUtf8("firuqtClass"));
        firuqtClass->resize(274, 265);
        actionGoNext = new QAction(firuqtClass);
        actionGoNext->setObjectName(QString::fromUtf8("actionGoNext"));
        actionGoBack = new QAction(firuqtClass);
        actionGoBack->setObjectName(QString::fromUtf8("actionGoBack"));
        gridLayout = new QGridLayout(firuqtClass);
        gridLayout->setSpacing(6);
        gridLayout->setMargin(11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        listView = new QListView(firuqtClass);
        listView->setObjectName(QString::fromUtf8("listView"));

        gridLayout->addWidget(listView, 0, 0, 1, 1);

        lineEdit = new QLineEdit(firuqtClass);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));

        gridLayout->addWidget(lineEdit, 1, 0, 1, 1);


        retranslateUi(firuqtClass);

        QMetaObject::connectSlotsByName(firuqtClass);
    } // setupUi

    void retranslateUi(QWidget *firuqtClass)
    {
        firuqtClass->setWindowTitle(QApplication::translate("firuqtClass", "firuqt", 0, QApplication::UnicodeUTF8));
        actionGoNext->setText(QApplication::translate("firuqtClass", "goNext", 0, QApplication::UnicodeUTF8));
        actionGoBack->setText(QApplication::translate("firuqtClass", "goBack", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(firuqtClass);
    } // retranslateUi

};

namespace Ui {
    class firuqtClass: public Ui_firuqtClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FIRUQT_H
