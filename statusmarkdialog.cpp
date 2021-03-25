#include "sportroomutils.h"
#include "statusmarkdialog.h"
#include "ui_statusmarkdialog.h"

StatusMarkDialog::StatusMarkDialog(MatchStatus* matchStatusModel, StateDatamodel *status, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StatusMarkDialog)
{
    ui->setupUi(this);
    ui->statusMarkFormWidget->setStatusModel(status);
    this->matchStatus = matchStatusModel;

    contentChanged();
    QObject::connect(matchStatus, &MatchStatus::contentChanged,
                     this, &StatusMarkDialog::contentChanged);
}

StatusMarkDialog::~StatusMarkDialog()
{
    delete ui;
}

void StatusMarkDialog::resizeEvent(QResizeEvent *event)
{
    SportRoomUtils::storeSize(this, "status_mark");

}


void StatusMarkDialog::contentChanged()
{
    ui->statusMarkFormWidget->setVisible(matchStatus->getElementState(0x04));
}
