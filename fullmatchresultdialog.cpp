#include "fullmatchresultdialog.h"
#include "ui_fullmatchresultdialog.h"
#include <QContextMenuEvent>
#include <QDebug>

FullMatchResultDialog::FullMatchResultDialog(MatchStatus* statusModel, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FullMatchResultDialog)
{
    ui->setupUi(this);

    ui->widget->setStatusModel(statusModel);
}

FullMatchResultDialog::~FullMatchResultDialog()
{
    delete ui;
}

void FullMatchResultDialog::resizeEvent(QResizeEvent *event)
{
    qDebug() << Q_FUNC_INFO << event->size();
}
