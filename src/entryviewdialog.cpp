#include "entryviewdialog.h"
#include "ui_entryviewdialog.h"
#include <QLabel>
#include <QFrame>
#include <QSpacerItem>

// ----------------------------------------------------------------------------

EntryViewDialog::EntryViewDialog(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::EntryViewDialog)
{
    m_ui->setupUi(this);

#ifdef __SYMBIAN32__
    showMaximized();
#else
    QRect rect = m_appUi.GetClientRect();
    setGeometry( rect );
#endif
}

// ----------------------------------------------------------------------------

EntryViewDialog::~EntryViewDialog()
{
    delete m_ui;
}

// ----------------------------------------------------------------------------

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

// ----------------------------------------------------------------------------

void EntryViewDialog::keyPressEvent( QKeyEvent* /*keyEvent*/ )
{
    close();
}

// ----------------------------------------------------------------------------

void EntryViewDialog::mousePressEvent( QMouseEvent* /*mouseEvent*/ )
{
    close();
}

// ----------------------------------------------------------------------------

int EntryViewDialog::exec( const QString& entry, const QStringList& translations )
{
    m_ui->laEntry->setText( entry );
    QString trans;
    for ( int i = 0; i < translations.count(); i++ )
    {
        if ( i > 0 )
        {
//            QFrame* line = new QFrame(m_ui->frame);
//            line->setObjectName(QString::fromUtf8("line%1").arg(i));
//            line->setFrameShape(QFrame::HLine);
//            line->setFrameShadow(QFrame::Sunken);
//            m_ui->verticalLayout_3->addWidget(line);
            trans.append( "<hr>" );
        }

//        QLabel* label = new QLabel(m_ui->frame);
//        label->setObjectName(QString::fromUtf8("label%1").arg(i));
//        label->setWordWrap( true );
//        label->setText( translations[i] );
//        m_ui->verticalLayout_3->addWidget(label);
        trans.append( translations[i] );
    }

//    QSpacerItem* verticalSpacer = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
//    m_ui->verticalLayout_3->addItem(verticalSpacer);

    m_ui->label->setText( trans );
    return QDialog::exec();
}
