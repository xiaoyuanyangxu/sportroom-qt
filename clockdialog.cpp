#include "clockdialog.h"
#include "ui_clockdialog.h"

#include <QTimer>
#include <QDebug>
#include <QDateTime>
#include <QSettings>

ClockDialog::ClockDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ClockDialog)
{
    ui->setupUi(this);

    setAttribute(Qt::WA_QuitOnClose, false);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &ClockDialog::onTimeout);
    timer->start(1000);
}

ClockDialog::~ClockDialog()
{
    delete ui;
}


void ClockDialog::onTimeout()
{
    QSettings settings;

    QColor otherColorText = QColor(settings.value("other_text","#000000").toString());

    qDebug() << Q_FUNC_INFO;
    QDateTime now = QDateTime::currentDateTime();
    ui->clockLabel->setText(now.toString("hh:mm:ss"));

    ui->clockLabel->setStyleSheet(QString("color : %1;").arg(otherColorText.name(QColor::HexRgb)));
}
