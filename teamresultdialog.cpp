#include "teamresultdialog.h"
#include "ui_teamresultdialog.h"

TeamResultDialog::TeamResultDialog(MatchStatus* statusModel, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TeamResultDialog)
{
    ui->setupUi(this);

    ui->widget->setStatusModel(statusModel);
}

TeamResultDialog::~TeamResultDialog()
{
    delete ui;
}
