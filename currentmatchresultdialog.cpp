#include "currentmatchresultdialog.h"
#include "ui_currentmatchresultdialog.h"

CurrentMatchResultDialog::CurrentMatchResultDialog(MatchStatus* statusModel, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CurrentMatchResultDialog)
{
    ui->setupUi(this);
    ui->widget->setStatusModel(statusModel);
}

CurrentMatchResultDialog::~CurrentMatchResultDialog()
{
    delete ui;
}
