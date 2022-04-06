#ifndef CLOCKDIALOG_H
#define CLOCKDIALOG_H

#include <QDialog>

namespace Ui {
class ClockDialog;
}

class ClockDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ClockDialog(QWidget *parent = nullptr);
    ~ClockDialog();

private slots:
    void onTimeout();

private:
    QTimer     * timer;

    Ui::ClockDialog *ui;
};

#endif // CLOCKDIALOG_H
