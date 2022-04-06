#include "statedatamodel.h"
#include "teamresultform.h"
#include "ui_teamresultform.h"

#include <QImageReader>
#include <QSettings>
#include "sportroomutils.h"

TeamResultForm::TeamResultForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TeamResultForm)
{
    ui->setupUi(this);

    nameColWidth = 240;
    totalWidth =  this->size().width() - 45;
    int totalHeight = this->size().height();
    ui->tableWidget->setColumnWidth(0, nameColWidth);
    ui->tableWidget->setColumnWidth(1, totalWidth - nameColWidth);
    ui->tableWidget->setRowHeight(0, totalHeight/2);
    ui->tableWidget->setRowHeight(1, totalHeight/2);
}

TeamResultForm::~TeamResultForm()
{
    delete ui;
}

void TeamResultForm::setStatusModel(MatchStatus *statusModel,  StateDatamodel * stateModel)
{
    this->statusModel = statusModel;
    this->stateModel  = stateModel;

    QObject::connect(statusModel, &MatchStatus::contentChanged,
                     this, &TeamResultForm::contentChanged);
    QObject::connect(stateModel, &StateDatamodel::contentChanged,
                     this, &TeamResultForm::contentChanged);
    contentChanged();
}

void TeamResultForm::contentChanged()
{
    QSettings settings;

    QColor primaryColorBack = QColor(settings.value("primary_back","#FFFFFF").toString());
    QColor primaryColorText = QColor(settings.value("primary_text","#000000").toString());

    QColor secondaryColorBack = QColor(settings.value("secondary_back","#FFFFFF").toString());
    QColor secondaryColorText = QColor(settings.value("secondary_text","#000000").toString());

    QTableWidgetItem * item;
    QString teamA, teamB;

    teamA = statusModel->getTeamAName();
    teamB = statusModel->getTeamBName();

    item = new QTableWidgetItem("  " + teamA + "");
    item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    item->setTextColor(primaryColorText);
    item->setBackgroundColor(primaryColorBack);
    ui->tableWidget->setItem(0,0, item);
    item = new QTableWidgetItem("  "  + teamB + "");
    item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    ui->tableWidget->setItem(1,0, item);
    item->setTextColor(primaryColorText);
    item->setBackgroundColor(primaryColorBack);

    int teamAResult, teamBResult;

    statusModel->getCurrentResult(teamAResult, teamBResult);

    item = new QTableWidgetItem("  " + QString::number(teamAResult) + "");
    item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    item->setTextColor(secondaryColorText);
    item->setBackgroundColor(secondaryColorBack);
    ui->tableWidget->setItem(0,1, item);

    item = new QTableWidgetItem("  "  + QString::number(teamBResult) + "");
    item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    ui->tableWidget->setItem(1,1, item);
    item->setTextColor(secondaryColorText);
    item->setBackgroundColor(secondaryColorBack);

    if (stateModel->getElementState(0x10) && teamAResult == 0 && teamBResult==0){
        ui->tableWidget->setColumnWidth(0, totalWidth);
        ui->tableWidget->setColumnWidth(1, 0);
    }else{
        ui->tableWidget->setColumnWidth(0, nameColWidth);
        ui->tableWidget->setColumnWidth(1, totalWidth - nameColWidth);
    }

    SportRoomUtils::drawImage(ui->teamBLogoLabel, statusModel->getTeamBLogoFile());
    SportRoomUtils::drawImage(ui->teamALogoLabel, statusModel->getTeamALogoFile());
}
