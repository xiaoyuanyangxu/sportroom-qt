#include "playerandteamsdialog.h"
#include "sportroomutils.h"
#include "ui_playerandteamsdialog.h"

#include <QContextMenuEvent>
#include <QSettings>
#include <QDebug>

PlayerAndTeamsDialog::PlayerAndTeamsDialog(PlayerDatamodel * dataModel, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PlayerAndTeamsDialog)
{
    ui->setupUi(this);

    ui->widget->setModel(dataModel);
}

PlayerAndTeamsDialog::~PlayerAndTeamsDialog()
{
    delete ui;
}

void PlayerAndTeamsDialog::resizeEvent(QResizeEvent *event)
{
    SportRoomUtils::storeSize(this, "player_and_teams");
}
