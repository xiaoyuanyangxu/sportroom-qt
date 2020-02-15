#include "fullmatchresultdialog.h"
#include "ui_fullmatchresultdialog.h"

FullMatchResultDialog::FullMatchResultDialog(MatchStatus* statusModel, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FullMatchResultDialog)
{
    ui->setupUi(this);

    ui->widget->setStatusModel(statusModel);
}

FullMatchResultDialog::~FullMatchResultDialog()
{
    delete ui;
}
