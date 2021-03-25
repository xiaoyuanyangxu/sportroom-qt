#ifndef TEAMRESULTDIALOG_H
#define TEAMRESULTDIALOG_H

#include "matchstatus.h"

#include <QDialog>

namespace Ui {
class TeamResultDialog;
}

class TeamResultDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TeamResultDialog(MatchStatus* statusModel, QWidget *parent = nullptr);
    ~TeamResultDialog();

public slots:
    void contentChanged();

private:
    Ui::TeamResultDialog *ui;

    MatchStatus* statusModel;
};

#endif // TEAMRESULTDIALOG_H
