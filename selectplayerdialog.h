#ifndef SELECTPLAYERDIALOG_H
#define SELECTPLAYERDIALOG_H

#include "playerdatamodel.h"

#include <QDialog>

namespace Ui {
class SelectPlayerDialog;
}

class SelectPlayerDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SelectPlayerDialog(PlayerDatamodel * model, QWidget *parent = nullptr);
    ~SelectPlayerDialog();

    QString getPlayerName();

private:
    Ui::SelectPlayerDialog *ui;
};

#endif // SELECTPLAYERDIALOG_H
