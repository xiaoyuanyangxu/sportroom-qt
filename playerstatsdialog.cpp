#include "playerstatsdialog.h"
#include "ui_playerstatsdialog.h"

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
