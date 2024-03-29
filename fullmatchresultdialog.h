#ifndef FULLMATCHRESULTDIALOG_H
#define FULLMATCHRESULTDIALOG_H

#include "matchstatus.h"

#include <QDialog>

namespace Ui {
class FullMatchResultDialog;
}

class FullMatchResultDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FullMatchResultDialog(MatchStatus *statusModel, QWidget *parent = nullptr);
    ~FullMatchResultDialog();

private:
    Ui::FullMatchResultDialog *ui;

    // QWidget interface
protected:
    void resizeEvent(QResizeEvent *event);
};

#endif // FULLMATCHRESULTDIALOG_H
