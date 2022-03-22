#ifndef STATUSMARKDIALOG_H
#define STATUSMARKDIALOG_H

#include <QDialog>

#include "statedatamodel.h"

#include "matchstatus.h"
namespace Ui {
class StatusMarkDialog;
}

class StatusMarkDialog : public QDialog
{
    Q_OBJECT

public:
    explicit StatusMarkDialog(MatchStatus* matchStatusModel, StateDatamodel *status, QWidget *parent = nullptr);
    ~StatusMarkDialog();

public slots:
    void contentChanged();

private:
    Ui::StatusMarkDialog * ui;

    MatchStatus          * matchStatus;
    StateDatamodel       * stateModel;

    // QWidget interface
protected:
    void resizeEvent(QResizeEvent *event);
};

#endif // STATUSMARKDIALOG_H
