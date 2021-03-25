#include "teamresultdialog.h"
#include "ui_teamresultdialog.h"

TeamResultDialog::TeamResultDialog(MatchStatus* statusModel, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TeamResultDialog)
{
    ui->setupUi(this);

    this->statusModel = statusModel;
    ui->widget->setStatusModel(statusModel);
    QObject::connect(statusModel, &MatchStatus::contentChanged,
                     this, &TeamResultDialog::contentChanged);
    contentChanged();
}

TeamResultDialog::~TeamResultDialog()
{
    delete ui;
}

void TeamResultDialog::contentChanged()
{
    ui->widget->setVisible(statusModel->getElementState(0x01));
}
