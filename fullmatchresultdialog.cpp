#include "fullmatchresultdialog.h"
#include "sportroomutils.h"
#include "ui_fullmatchresultdialog.h"
#include <QContextMenuEvent>
#include <QDebug>
#include <QSettings>

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
    SportRoomUtils::storeSize(this, "full_match_result");
}
