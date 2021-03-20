#include "sportroomutils.h"
#include "statusmarkdialog.h"
#include "ui_statusmarkdialog.h"

StatusMarkDialog::StatusMarkDialog(StateDatamodel *status, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StatusMarkDialog)
{
    ui->setupUi(this);
    ui->statusMarkFormWidget->setStatusModel(status);
}

StatusMarkDialog::~StatusMarkDialog()
{
    delete ui;
}

void StatusMarkDialog::resizeEvent(QResizeEvent *event)
{
    SportRoomUtils::storeSize(this, "status_mark");
}
