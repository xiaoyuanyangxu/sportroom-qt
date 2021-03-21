#include "multifuntionaldialog.h"
#include "sportroomutils.h"
#include "ui_multifuntionaldialog.h"
#include <QtDebug>

MultifuntionalDialog::MultifuntionalDialog(MatchStatus *statusModel, PlayerDatamodel *playerDatamodel, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MultifuntionalDialog)
{
    ui->setupUi(this);

    ui->fullMatchResultWidget->setStatusModel(statusModel);
    ui->playerStatsWidget->setModels(statusModel, playerDatamodel);
    //ui->stackedWidget->setBackgroundRole(QPalette::Text);
    //ui->stackedWidget->setSta
    //ui->fullMatchResultWidget->setBackgroundRole(QPalette::Text);


    this->matchStatusModel = statusModel;
    this->playerDatamodel  = playerDatamodel;

    QObject::connect(matchStatusModel, &MatchStatus::contentChanged,
                     this, &MultifuntionalDialog::contentChanged);

    contentChanged();
}

MultifuntionalDialog::~MultifuntionalDialog()
{
    delete ui;
}

void MultifuntionalDialog::contentChanged()
{
    int state = matchStatusModel->getMultifunctionaScreenState();

    qDebug() << Q_FUNC_INFO << state;

    ui->fullMatchResultWidget->setVisible(state == 1);
    ui->playerStatsWidget->setVisible(state == 2);
    if (state==2) {
        ui->playerStatsWidget->resize(this->width(), this->height());
    }
    ui->fullMatchResultWidget->setVisible(state == 1);

    ui->playerStatsWidget->setVisible(state == 2);
    ui->imageLabel->setVisible(state == 3);
    ui->controlFrame->setVisible(state == 4);
}

void MultifuntionalDialog::resizeEvent(QResizeEvent *event)
{
    SportRoomUtils::storeSize(this, "multifunctional_dialog");
}
