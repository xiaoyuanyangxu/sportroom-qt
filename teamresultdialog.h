#ifndef TEAMRESULTDIALOG_H
#define TEAMRESULTDIALOG_H

#include "matchstatus.h"
#include "statedatamodel.h"

#include <QDialog>

namespace Ui {
class TeamResultDialog;
}

class TeamResultDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TeamResultDialog(MatchStatus* matchModel,
                              StateDatamodel *stateModel,
                              QWidget *parent = nullptr);
    ~TeamResultDialog();

public slots:
    void contentChanged();

private:
    Ui::TeamResultDialog *ui;

    MatchStatus * matchModel;
    StateDatamodel * stateModel;
};

#endif // TEAMRESULTDIALOG_H
