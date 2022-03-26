#include "settingdialog.h"
#include "ui_settingdialog.h"
#include <QColorDialog>
#include <QFileDialog>
#include <QSettings>
#include <QtDebug>

SettingDialog::SettingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingDialog)
{
    ui->setupUi(this);

    setAttribute(Qt::WA_QuitOnClose, false);

    updateLabels();
}

SettingDialog::~SettingDialog()
{
    delete ui;
}

void SettingDialog::updateLabels()
{
    QSettings settings;

    QString name = "primary_back";
    QString text = "primary_text";

    QColor colorBack = QColor(settings.value(name,"#FFFFFF").toString());
    QColor colorText = QColor(settings.value(text,"#000000").toString());

    ui->primaryBackPushButton->setStyleSheet(QString("background-color : %1;").arg(colorBack.name(QColor::HexRgb)));
    ui->primaryTextPushButton->setStyleSheet(QString("background-color : %1;").arg(colorText.name(QColor::HexRgb)));

    ///
    name = "secondary_back";
    text = "secondary_text";

    colorBack = QColor(settings.value(name,"#FFFFFF").toString());
    colorText = QColor(settings.value(text,"#000000").toString());

    ui->secondaryBackPushButton->setStyleSheet(QString("background-color : %1;").arg(colorBack.name(QColor::HexRgb)));
    ui->secondaryTextPushButton->setStyleSheet(QString("background-color : %1;").arg(colorText.name(QColor::HexRgb)));

    ///
    name = "other_back";
    text = "other_text";

    colorBack = QColor(settings.value(name,"#FFFFFF").toString());
    colorText = QColor(settings.value(text,"#000000").toString());

    ui->otherBackPushButton->setStyleSheet(QString("background-color : %1;").arg(colorBack.name(QColor::HexRgb)));
    ui->otherTextPushButton->setStyleSheet(QString("background-color : %1;").arg(colorText.name(QColor::HexRgb)));

    ui->assetFolderLineEdit->setText(settings.value("asset_folder","").toString());

}

void SettingDialog::on_pushButton_clicked()
{
    accept();
}

void SettingDialog::on_primaryBackPushButton_clicked()
{
    QSettings settings;
    QString name = "primary_back";

    QColor color = QColorDialog::getColor(QColor(settings.value(name,"#FFFFFF").toString()),this);

    if (color.isValid()) {
        qDebug() << Q_FUNC_INFO << "Selected color: "<<color.name(QColor::HexRgb);
        settings.setValue(name, color.name(QColor::HexRgb));
        ui->primaryBackPushButton->setStyleSheet(QString("background-color : %1;").arg(color.name(QColor::HexRgb)));
        updateLabels();
    }
}

void SettingDialog::on_primaryTextPushButton_clicked()
{
    QSettings settings;
    QString name = "primary_text";

    QColor color = QColorDialog::getColor(QColor(settings.value(name,"#FFFFFF").toString()),this);

    if (color.isValid()) {
        qDebug() << Q_FUNC_INFO << "Selected color: "<<color.name(QColor::HexRgb);
        settings.setValue(name, color.name(QColor::HexRgb));
        ui->primaryTextPushButton->setStyleSheet(QString("background-color : %1;").arg(color.name(QColor::HexRgb)));
        updateLabels();
    }
}


void SettingDialog::on_secondaryBackPushButton_clicked()
{
    QSettings settings;
    QString name = "secondary_back";

    QColor color = QColorDialog::getColor(QColor(settings.value(name,"#FFFFFF").toString()),this);

    if (color.isValid()) {
        qDebug() << Q_FUNC_INFO << "Selected color: "<<color.name(QColor::HexRgb);
        settings.setValue(name, color.name(QColor::HexRgb));
        ui->secondaryBackPushButton->setStyleSheet(QString("background-color : %1;").arg(color.name(QColor::HexRgb)));
        updateLabels();
    }
}

void SettingDialog::on_secondaryTextPushButton_clicked()
{
    QSettings settings;
    QString name = "secondary_text";

    QColor color = QColorDialog::getColor(QColor(settings.value(name,"#FFFFFF").toString()),this);

    if (color.isValid()) {
        qDebug() << Q_FUNC_INFO << "Selected color: "<<color.name(QColor::HexRgb);
        settings.setValue(name, color.name(QColor::HexRgb));
        ui->secondaryTextPushButton->setStyleSheet(QString("background-color : %1;").arg(color.name(QColor::HexRgb)));
        updateLabels();
    }
}

void SettingDialog::on_otherBackPushButton_clicked()
{
    QSettings settings;
    QString name = "other_back";

    QColor color = QColorDialog::getColor(QColor(settings.value(name,"#FFFFFF").toString()),this);

    if (color.isValid()) {
        qDebug() << Q_FUNC_INFO << "Selected color: "<<color.name(QColor::HexRgb);
        settings.setValue(name, color.name(QColor::HexRgb));
        ui->otherBackPushButton->setStyleSheet(QString("background-color : %1;").arg(color.name(QColor::HexRgb)));
        updateLabels();
    }
}



void SettingDialog::on_otherTextPushButton_clicked()
{
    QSettings settings;
    QString name = "other_text";

    QColor color = QColorDialog::getColor(QColor(settings.value(name,"#FFFFFF").toString()),this);

    if (color.isValid()) {
        qDebug() << Q_FUNC_INFO << "Selected color: "<<color.name(QColor::HexRgb);
        settings.setValue(name, color.name(QColor::HexRgb));
        ui->otherTextPushButton->setStyleSheet(QString("background-color : %1;").arg(color.name(QColor::HexRgb)));
        updateLabels();
    }
}

void SettingDialog::on_assetFolderPushButton_clicked()
{
    QSettings settings;

    QString dir = QFileDialog::getExistingDirectory(this,
                                                    "",
                                                    settings.value("asset_folder","").toString(),
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);
    if (!dir.isEmpty())
    {
        settings.setValue("asset_folder", dir);
        ui->assetFolderLineEdit->setText(dir);
    }
}

void SettingDialog::on_primaryDefaultPushButton_clicked()
{
    QSettings settings;
    QString name = "primary_back";
    QColor color = QColor("#003f72");

    settings.setValue(name, color);
    ui->primaryBackPushButton->setStyleSheet(QString("background-color : %1;").arg(color.name(QColor::HexRgb)));

    name = "primary_text";
    color = QColor("#DAE5ED");

    settings.setValue(name, color);
    ui->primaryTextPushButton->setStyleSheet(QString("background-color : %1;").arg(color.name(QColor::HexRgb)));

    updateLabels();
}

void SettingDialog::on_secondaryDefaultPushButton_clicked()
{
    QSettings settings;
    QString name = "secondary_back";
    QColor color = QColor("#DAE5ED");

    settings.setValue(name, color);
    ui->secondaryBackPushButton->setStyleSheet(QString("background-color : %1;").arg(color.name(QColor::HexRgb)));

    name = "secondary_text";
    color = QColor("#003f72");

    settings.setValue(name, color);
    ui->secondaryTextPushButton->setStyleSheet(QString("background-color : %1;").arg(color.name(QColor::HexRgb)));

    updateLabels();
}

void SettingDialog::on_otherDefaultPushButton_clicked()
{
    QSettings settings;
    QString name = "other_back";
    QColor color = QColor("#003f72");

    settings.setValue(name, color);
    ui->otherBackPushButton->setStyleSheet(QString("background-color : %1;").arg(color.name(QColor::HexRgb)));

    name = "pother_text";
    color = QColor("#DAE5ED");

    settings.setValue(name, color);
    ui->otherTextPushButton->setStyleSheet(QString("background-color : %1;").arg(color.name(QColor::HexRgb)));

    updateLabels();
}
