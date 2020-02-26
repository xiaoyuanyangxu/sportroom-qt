#ifndef PLAYERANDTEAMSFORM_H
#define PLAYERANDTEAMSFORM_H

#include <QWidget>

#include "playerdatamodel.h"

namespace Ui {
class PlayerAndTeamsForm;
}

class PlayerAndTeamsForm : public QWidget
{
    Q_OBJECT

public:
    explicit PlayerAndTeamsForm(QWidget *parent = nullptr);
    ~PlayerAndTeamsForm();

    void setModel(PlayerDatamodel * model);


private slots:
    void on_importPushButton_clicked();

    void on_deletePushButton_clicked();

private:
    Ui::PlayerAndTeamsForm *ui;

    PlayerDatamodel * model;
};

#endif // PLAYERANDTEAMSFORM_H
