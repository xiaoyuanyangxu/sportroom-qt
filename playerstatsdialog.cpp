#include "playerstatsdialog.h"
#include "ui_playerstatsdialog.h"

#include <QContextMenuEvent>
#include <QSettings>
#include <QDebug>
#include "sportroomutils.h"

PlayerStatsDialog::PlayerStatsDialog(MatchStatus *status, PlayerDatamodel *players, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PlayerStatsDialog)
{
    ui->setupUi(this);

    ui->widget->setModels(status, players);

}

PlayerStatsDialog::~PlayerStatsDialog()
{
    delete ui;
}

void PlayerStatsDialog::resizeEvent(QResizeEvent *event)
{
    SportRoomUtils::storeSize(this, "player_stats");
}
