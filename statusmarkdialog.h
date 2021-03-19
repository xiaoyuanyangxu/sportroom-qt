#ifndef STATUSMARKDIALOG_H
#define STATUSMARKDIALOG_H

#include <QDialog>

#include "statedatamodel.h"

namespace Ui {
class StatusMarkDialog;
}

class StatusMarkDialog : public QDialog
{
    Q_OBJECT

public:
    explicit StatusMarkDialog(StateDatamodel *status, QWidget *parent = nullptr);
    ~StatusMarkDialog();

private:
    Ui::StatusMarkDialog *ui;
};

#endif // STATUSMARKDIALOG_H
