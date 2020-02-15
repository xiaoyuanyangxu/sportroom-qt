#ifndef CURRENTMATCHRESULTDIALOG_H
#define CURRENTMATCHRESULTDIALOG_H

#include "matchstatus.h"

#include <QDialog>

namespace Ui {
class CurrentMatchResultDialog;
}

class CurrentMatchResultDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CurrentMatchResultDialog(MatchStatus* statusModel, QWidget *parent = nullptr);
    ~CurrentMatchResultDialog();

private:
    Ui::CurrentMatchResultDialog *ui;
};

#endif // CURRENTMATCHRESULTDIALOG_H
