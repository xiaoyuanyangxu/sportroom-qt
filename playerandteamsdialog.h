#ifndef PLAYERANDTEAMSDIALOG_H
#define PLAYERANDTEAMSDIALOG_H

#include <QDialog>

#include "playerdatamodel.h"

namespace Ui {
class PlayerAndTeamsDialog;
}

class PlayerAndTeamsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PlayerAndTeamsDialog(PlayerDatamodel * dataModel, QWidget *parent = nullptr);
    ~PlayerAndTeamsDialog();

private:
    Ui::PlayerAndTeamsDialog *ui;
};

#endif // PLAYERANDTEAMSDIALOG_H
