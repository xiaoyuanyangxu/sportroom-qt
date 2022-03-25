#ifndef SETTINGDIALOG_H
#define SETTINGDIALOG_H

#include <QDialog>

namespace Ui {
class SettingDialog;
}

class SettingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingDialog(QWidget *parent = nullptr);
    ~SettingDialog();

private:
    void updateLabels();

private slots:
    void on_primaryBackPushButton_clicked();

    void on_pushButton_clicked();

    void on_primaryTextPushButton_clicked();

    void on_secondaryBackPushButton_clicked();

    void on_secondaryTextPushButton_clicked();

    void on_otherBackPushButton_clicked();

    void on_otherTextPushButton_clicked();

    void on_assetFolderPushButton_clicked();

private:
    Ui::SettingDialog *ui;
};

#endif // SETTINGDIALOG_H
