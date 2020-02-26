#include "playerandteamsdialog.h"
#include "ui_playerandteamsdialog.h"

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
