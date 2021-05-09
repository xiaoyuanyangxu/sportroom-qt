#include "statusmarkform.h"
#include "ui_statusmarkform.h"

StatusMarkForm::StatusMarkForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StatusMarkForm)
{
    ui->setupUi(this);
}

StatusMarkForm::~StatusMarkForm()
{
    delete ui;
}

void StatusMarkForm::setStatusModel(StateDatamodel *statusModel)
{
   this->statusModel = statusModel;
   contentChanged();
   QObject::connect(statusModel, &StateDatamodel::contentChanged,
                    this, &StatusMarkForm::contentChanged);

}

void StatusMarkForm::contentChanged()
{
    QString blackStyle = "background-color: rgb(0, 0, 0);";
    QString whiteStyle = "background-color: rgb(255, 255, 255);";

    int status = statusModel->getCurrentStatus();
    if (status & 0x01) {
        ui->frame4->setStyleSheet(whiteStyle);
    }else{
        ui->frame4->setStyleSheet(blackStyle);
    }
    if (status & 0x02) {
        ui->frame3->setStyleSheet(whiteStyle);
    }else{
        ui->frame3->setStyleSheet(blackStyle);
    }
    if (status & 0x04) {
        ui->frame2->setStyleSheet(whiteStyle);
    }else{
        ui->frame2->setStyleSheet(blackStyle);
    }
    if (status & 0x08) {
        ui->frame1->setStyleSheet(whiteStyle);
    }else{
        ui->frame1->setStyleSheet(blackStyle);
    }
}
