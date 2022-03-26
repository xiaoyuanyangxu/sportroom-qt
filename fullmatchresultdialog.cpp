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

    QObject::connect(statusModel, &MatchStatus::contentChanged,
                     this, &FullMatchResultDialog::contentChanged);

    setAttribute(Qt::WA_QuitOnClose, false);

    contentChanged();

}

FullMatchResultDialog::~FullMatchResultDialog()
{
    delete ui;
}

void FullMatchResultDialog::resizeEvent(QResizeEvent *event)
{
    SportRoomUtils::storeSize(this, "full_match_result");
}

void FullMatchResultDialog::contentChanged()
{
    QSettings settings;

    QColor colorBack = QColor(settings.value("other_back","#FFFFFF").toString());
    this->setStyleSheet(QString("background-color : %1;").arg(colorBack.name(QColor::HexRgb)));
}
