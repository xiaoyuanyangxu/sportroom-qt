#include "teamresultform.h"
#include "ui_teamresultform.h"

#include <QImageReader>
#include "sportroomutils.h"

TeamResultForm::TeamResultForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TeamResultForm)
{
    ui->setupUi(this);

    int nameSize = 230;
    int totalWidth =  this->size().width() - 45;
    int totalHeight = this->size().height();
    ui->tableWidget->setColumnWidth(0, nameSize);
    ui->tableWidget->setColumnWidth(1, totalWidth - nameSize);
    ui->tableWidget->setRowHeight(0, totalHeight/2);
    ui->tableWidget->setRowHeight(1, totalHeight/2);
}

TeamResultForm::~TeamResultForm()
{
    delete ui;
}

void TeamResultForm::setStatusModel(MatchStatus *statusModel)
{
    this->statusModel = statusModel;

    QObject::connect(statusModel, &MatchStatus::contentChanged,
                     this, &TeamResultForm::contentChanged);
    contentChanged();
}

void TeamResultForm::contentChanged()
{
    QTableWidgetItem * item;
    QColor color1(0,63,114);
    QColor color2(218,229,237);

    QString teamA, teamB;
    teamA = statusModel->getTeamAName();
    teamB = statusModel->getTeamBName();

    item = new QTableWidgetItem("  " + teamA + "");
    item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    item->setTextColor(color2);
    ui->tableWidget->setItem(0,0, item);
    item = new QTableWidgetItem("  "  + teamB + "");
    item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    ui->tableWidget->setItem(1,0, item);
    item->setTextColor(color2);

    int teamAResult, teamBResult;

    statusModel->getCurrentResult(teamAResult, teamBResult);

    item = new QTableWidgetItem("  " + QString::number(teamAResult) + "");
    item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    item->setTextColor(color1);
    ui->tableWidget->setItem(0,1, item);
    item = new QTableWidgetItem("  "  + QString::number(teamBResult) + "");
    item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    ui->tableWidget->setItem(1,1, item);
    item->setTextColor(color1);

    SportRoomUtils::drawImage(ui->teamBLogoLabel, statusModel->getTeamBLogoFile());
    SportRoomUtils::drawImage(ui->teamALogoLabel, statusModel->getTeamALogoFile());
}
