#include "helpform.h"
#include "ui_helpform.h"

#include "gamewidget.h"

HelpForm::HelpForm(const QString &text, QWidget *parent) :
    QDialog(parent, Qt::FramelessWindowHint),
    ui(new Ui::HelpForm)
{
    ui->setupUi(this);

    QString helpfile = GameWidget::getResourcePath() + "/help";
    QStringList path; path << helpfile;
    ui->teHelp->setSearchPaths(path);
    ui->teHelp->setHtml(text);

    setFixedSize(700,500);
}

HelpForm::~HelpForm()
{
    delete ui;
}

void HelpForm::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
