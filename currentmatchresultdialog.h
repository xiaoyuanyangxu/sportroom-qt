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

public slots:
    void contentChanged();

private:
    Ui::CurrentMatchResultDialog *ui;

    MatchStatus* statusModel;
};

#endif // CURRENTMATCHRESULTDIALOG_H
