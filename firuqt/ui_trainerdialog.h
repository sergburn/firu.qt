/********************************************************************************
** Form generated from reading ui file 'trainerdialog.ui'
**
** Created: Sun Apr 5 21:22:05 2009
**      by: Qt User Interface Compiler version 4.5.0
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_TRAINERDIALOG_H
#define UI_TRAINERDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QListWidget>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_TrainerDialog
{
public:
    QVBoxLayout *verticalLayout_3;
    QFrame *frame;
    QVBoxLayout *verticalLayout_2;
    QLabel *laQuestion;
    QFrame *frmKeypad;
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QGridLayout *gridLayout;
    QLabel *la4;
    QLabel *la7;
    QLabel *la11;
    QLabel *la5;
    QLabel *la2;
    QLabel *la6;
    QLabel *la8;
    QLabel *la0;
    QLabel *la12;
    QLabel *la9;
    QLabel *la3;
    QLabel *la1;
    QListWidget *listAnswers;

    void setupUi(QDialog *TrainerDialog)
    {
        if (TrainerDialog->objectName().isEmpty())
            TrainerDialog->setObjectName(QString::fromUtf8("TrainerDialog"));
        TrainerDialog->resize(240, 320);
        verticalLayout_3 = new QVBoxLayout(TrainerDialog);
        verticalLayout_3->setSpacing(3);
        verticalLayout_3->setMargin(3);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        frame = new QFrame(TrainerDialog);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        verticalLayout_2 = new QVBoxLayout(frame);
        verticalLayout_2->setSpacing(3);
        verticalLayout_2->setMargin(3);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        laQuestion = new QLabel(frame);
        laQuestion->setObjectName(QString::fromUtf8("laQuestion"));
        laQuestion->setFrameShape(QFrame::Box);
        laQuestion->setAlignment(Qt::AlignCenter);
        laQuestion->setWordWrap(true);

        verticalLayout_2->addWidget(laQuestion);


        verticalLayout_3->addWidget(frame);

        frmKeypad = new QFrame(TrainerDialog);
        frmKeypad->setObjectName(QString::fromUtf8("frmKeypad"));
        frmKeypad->setFrameShape(QFrame::StyledPanel);
        frmKeypad->setFrameShadow(QFrame::Raised);
        verticalLayout = new QVBoxLayout(frmKeypad);
        verticalLayout->setSpacing(3);
        verticalLayout->setMargin(3);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        label = new QLabel(frmKeypad);
        label->setObjectName(QString::fromUtf8("label"));
        label->setFrameShape(QFrame::Box);
        label->setAlignment(Qt::AlignCenter);
        label->setWordWrap(true);

        verticalLayout->addWidget(label);

        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        la4 = new QLabel(frmKeypad);
        la4->setObjectName(QString::fromUtf8("la4"));
        la4->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(la4, 1, 0, 1, 1);

        la7 = new QLabel(frmKeypad);
        la7->setObjectName(QString::fromUtf8("la7"));
        la7->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(la7, 2, 0, 1, 1);

        la11 = new QLabel(frmKeypad);
        la11->setObjectName(QString::fromUtf8("la11"));
        la11->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(la11, 3, 0, 1, 1);

        la5 = new QLabel(frmKeypad);
        la5->setObjectName(QString::fromUtf8("la5"));
        la5->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(la5, 1, 1, 1, 1);

        la2 = new QLabel(frmKeypad);
        la2->setObjectName(QString::fromUtf8("la2"));
        la2->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(la2, 0, 1, 1, 1);

        la6 = new QLabel(frmKeypad);
        la6->setObjectName(QString::fromUtf8("la6"));
        la6->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(la6, 1, 2, 1, 1);

        la8 = new QLabel(frmKeypad);
        la8->setObjectName(QString::fromUtf8("la8"));
        la8->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(la8, 2, 1, 1, 1);

        la0 = new QLabel(frmKeypad);
        la0->setObjectName(QString::fromUtf8("la0"));
        la0->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(la0, 3, 1, 1, 1);

        la12 = new QLabel(frmKeypad);
        la12->setObjectName(QString::fromUtf8("la12"));
        la12->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(la12, 3, 2, 1, 1);

        la9 = new QLabel(frmKeypad);
        la9->setObjectName(QString::fromUtf8("la9"));
        la9->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(la9, 2, 2, 1, 1);

        la3 = new QLabel(frmKeypad);
        la3->setObjectName(QString::fromUtf8("la3"));
        la3->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(la3, 0, 2, 1, 1);

        la1 = new QLabel(frmKeypad);
        la1->setObjectName(QString::fromUtf8("la1"));
        la1->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(la1, 0, 0, 1, 1);


        verticalLayout->addLayout(gridLayout);


        verticalLayout_3->addWidget(frmKeypad);

        listAnswers = new QListWidget(TrainerDialog);
        listAnswers->setObjectName(QString::fromUtf8("listAnswers"));

        verticalLayout_3->addWidget(listAnswers);


        retranslateUi(TrainerDialog);

        QMetaObject::connectSlotsByName(TrainerDialog);
    } // setupUi

    void retranslateUi(QDialog *TrainerDialog)
    {
        TrainerDialog->setWindowTitle(QApplication::translate("TrainerDialog", "Dialog", 0, QApplication::UnicodeUTF8));
        laQuestion->setText(QApplication::translate("TrainerDialog", "question", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("TrainerDialog", "TextLabel", 0, QApplication::UnicodeUTF8));
        la4->setText(QApplication::translate("TrainerDialog", "\320\272\320\273\320\274", 0, QApplication::UnicodeUTF8));
        la7->setText(QApplication::translate("TrainerDialog", "\321\203\321\204\321\205", 0, QApplication::UnicodeUTF8));
        la11->setText(QApplication::translate("TrainerDialog", "List", 0, QApplication::UnicodeUTF8));
        la5->setText(QApplication::translate("TrainerDialog", "\320\275\320\276\320\277", 0, QApplication::UnicodeUTF8));
        la2->setText(QApplication::translate("TrainerDialog", "\320\263\320\264\320\265\321\221", 0, QApplication::UnicodeUTF8));
        la6->setText(QApplication::translate("TrainerDialog", "\321\200\321\201\321\202", 0, QApplication::UnicodeUTF8));
        la8->setText(QApplication::translate("TrainerDialog", "\321\206\321\207\321\210", 0, QApplication::UnicodeUTF8));
        la0->setText(QApplication::translate("TrainerDialog", "\321\215\321\216\321\217", 0, QApplication::UnicodeUTF8));
        la12->setText(QApplication::translate("TrainerDialog", "Tell", 0, QApplication::UnicodeUTF8));
        la9->setText(QApplication::translate("TrainerDialog", "\321\211\321\212\321\213\321\212", 0, QApplication::UnicodeUTF8));
        la3->setText(QApplication::translate("TrainerDialog", "\320\266\320\267\320\270\320\271", 0, QApplication::UnicodeUTF8));
        la1->setText(QApplication::translate("TrainerDialog", "\320\260\320\261\320\262", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(TrainerDialog);
    } // retranslateUi

};

namespace Ui {
    class TrainerDialog: public Ui_TrainerDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TRAINERDIALOG_H
