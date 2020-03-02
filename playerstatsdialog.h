#ifndef PLAYERSTATSDIALOG_H
#define PLAYERSTATSDIALOG_H

#include "matchstatus.h"
#include "playerdatamodel.h"

#include <QDialog>

namespace Ui {
class PlayerStatsDialog;
}

class PlayerStatsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PlayerStatsDialog(MatchStatus * status, PlayerDatamodel *players, QWidget *parent = nullptr);
    ~PlayerStatsDialog();

private:
    Ui::PlayerStatsDialog *ui;

    // QWidget interface
protected:
    void resizeEvent(QResizeEvent *event);
};

#endif // PLAYERSTATSDIALOG_H
