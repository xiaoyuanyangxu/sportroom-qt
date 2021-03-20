#ifndef MULTIFUNTIONALDIALOG_H
#define MULTIFUNTIONALDIALOG_H

#include "matchstatus.h"

#include <QDialog>

namespace Ui {
class MultifuntionalDialog;
}

class MultifuntionalDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MultifuntionalDialog(MatchStatus *statusModel, QWidget *parent = nullptr);
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
};

#endif // MULTIFUNTIONALDIALOG_H
