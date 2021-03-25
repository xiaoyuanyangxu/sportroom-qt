#include "currentmatchresultdialog.h"
#include "ui_currentmatchresultdialog.h"

CurrentMatchResultDialog::CurrentMatchResultDialog(MatchStatus* statusModel, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CurrentMatchResultDialog)
{
    ui->setupUi(this);
    ui->widget->setStatusModel(statusModel);
    this->statusModel = statusModel;
    QObject::connect(statusModel, &MatchStatus::contentChanged,
                     this, &CurrentMatchResultDialog::contentChanged);
    contentChanged();
}

CurrentMatchResultDialog::~CurrentMatchResultDialog()
{
    delete ui;
}

void CurrentMatchResultDialog::contentChanged()
{
    ui->widget->setVisible(statusModel->getElementState(0x02));
}

