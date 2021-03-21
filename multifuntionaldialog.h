#ifndef MULTIFUNTIONALDIALOG_H
#define MULTIFUNTIONALDIALOG_H

#include "matchstatus.h"
#include "playerdatamodel.h"

#include <QDialog>

namespace Ui {
class MultifuntionalDialog;
}

class MultifuntionalDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MultifuntionalDialog(MatchStatus *statusModel, PlayerDatamodel *playerDatamodel, QWidget *parent = nullptr);
    ~MultifuntionalDialog();

public slots:
    void contentChanged();

private:
    Ui::MultifuntionalDialog *ui;

    // QWidget interface
protected:
    void resizeEvent(QResizeEvent *event);

private:
    MatchStatus *           matchStatusModel;
    PlayerDatamodel*        playerDatamodel;
};

#endif // MULTIFUNTIONALDIALOG_H
