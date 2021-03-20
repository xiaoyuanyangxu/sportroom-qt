#include "multifuntionaldialog.h"
#include "sportroomutils.h"
#include "ui_multifuntionaldialog.h"
#include <QtDebug>

MultifuntionalDialog::MultifuntionalDialog(MatchStatus *statusModel, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MultifuntionalDialog)
{
    ui->setupUi(this);
    ui->widget->setStatusModel(statusModel);

    matchStatusModel = statusModel;

    QObject::connect(matchStatusModel, &MatchStatus::contentChanged,
                     this, &MultifuntionalDialog::contentChanged);
}

MultifuntionalDialog::~MultifuntionalDialog()
{
    delete ui;
}

void MultifuntionalDialog::contentChanged()
{
    int state = matchStatusModel->getMultifunctionaScreenState();

    qDebug() << Q_FUNC_INFO << state;

    if (state == 0) {
        ui->widget->setVisible(false);
    }else{

        ui->widget->setVisible(true);
    }

}

void MultifuntionalDialog::resizeEvent(QResizeEvent *event)
{
    SportRoomUtils::storeSize(this, "multifunctional_dialog");
}
