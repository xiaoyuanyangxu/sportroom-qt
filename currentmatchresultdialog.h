#ifndef CURRENTMATCHRESULTDIALOG_H
#define CURRENTMATCHRESULTDIALOG_H

#include "matchstatus.h"
#include "statedatamodel.h"

#include <QDialog>

namespace Ui {
class CurrentMatchResultDialog;
}

class CurrentMatchResultDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CurrentMatchResultDialog(MatchStatus* statusModel,
                                      StateDatamodel* stateModel,
                                      QWidget *parent = nullptr);
    ~CurrentMatchResultDialog();

public slots:
    void contentChanged();

private:
    Ui::CurrentMatchResultDialog *ui;

    MatchStatus* matchModel;
    StateDatamodel* stateModel;
};

#endif // CURRENTMATCHRESULTDIALOG_H
