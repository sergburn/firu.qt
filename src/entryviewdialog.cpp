#include "entryviewdialog.h"
#include "ui_entryviewdialog.h"
#include <QLabel>
#include <QFrame>

EntryViewDialog::EntryViewDialog(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::EntryViewDialog)
{
    m_ui->setupUi(this);
}

EntryViewDialog::~EntryViewDialog()
{
    delete m_ui;
}

void EntryViewDialog::changeEvent(QEvent *e)
{
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

int EntryViewDialog::exec( const Word& word, const QList<Translation>& translations )
{
    m_ui->laEntry->setText( word.getText() );
    if ( translations.count() > 0 )
    {
        m_ui->label->setText( translations[0].getText() );
        for ( int i = 1; i < translations.count(); i++ )
        {
            QLabel* label = new QLabel(m_ui->scrollAreaWidgetContents);
            label->setObjectName(QString::fromUtf8("label%1").arg(i));
            m_ui->verticalLayout_2->addWidget(label);

            QFrame* line = new QFrame(m_ui->scrollAreaWidgetContents);
            line->setObjectName(QString::fromUtf8("line%1").arg(i));
            line->setFrameShape(QFrame::HLine);
            line->setFrameShadow(QFrame::Sunken);
            m_ui->verticalLayout_2->addWidget(line);
        }
    }
    return QDialog::exec();
}
