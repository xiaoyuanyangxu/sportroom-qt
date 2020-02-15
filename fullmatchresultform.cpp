#include "fullmatchresultform.h"
#include "ui_fullmatchresultform.h"

#include <QDate>
#include <QImageReader>

FullMatchResultForm::FullMatchResultForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FullMatchResultForm)
{
    ui->setupUi(this);
    QDate today = QDate::currentDate();
    ui->dayLabel->setText(today.toString("dd/MM/yyyy"));
}

FullMatchResultForm::~FullMatchResultForm()
{
    delete ui;
}

void FullMatchResultForm::setStatusModel(MatchStatus *statusModel)
{

    this->statusModel = statusModel;
    ui->matchResultTableView->setModel(statusModel);


    ui->matchResultTableView->verticalHeader()->hide();
    ui->matchResultTableView->setShowGrid(true);

    ui->matchResultTableView->setSelectionMode(QAbstractItemView::NoSelection);
    ui->matchResultTableView->setSelectionBehavior(QAbstractItemView::SelectItems);
    ui->matchResultTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);


    ui->matchResultTableView->setColumnWidth(0, 20);
    ui->matchResultTableView->setColumnWidth(2, 20);
    ui->matchResultTableView->setColumnWidth(4, 60);
    ui->matchResultTableView->setColumnWidth(5, 60);
    ui->matchResultTableView->setColumnWidth(6, 60);
    ui->matchResultTableView->setColumnWidth(7, 60);
    ui->matchResultTableView->setColumnWidth(8, 60);
    ui->matchResultTableView->setColumnWidth(9, 60);
    ui->matchResultTableView->setColumnWidth(10, 60);
    //ui->matchResultTableView->setRowHeight(5, 60);

    QHeaderView *header = ui->matchResultTableView->horizontalHeader();
    header->setSectionResizeMode(1, QHeaderView::Stretch);
    header->setSectionResizeMode(3, QHeaderView::Stretch);


    QHeaderView *verticalHeader = ui->matchResultTableView->verticalHeader();
    verticalHeader->sectionResizeMode(QHeaderView::Fixed);
    verticalHeader->setDefaultSectionSize(26);


    QObject::connect(statusModel, &MatchStatus::contentChanged,
                     this, &FullMatchResultForm::contentChanged);
    contentChanged();

}

void FullMatchResultForm::drawImage(QLabel *label, QString uri)
{
    if (uri.isEmpty()) return;

    int w = label->width();
    int h = label->height();

    QImageReader imageReader(uri);
    imageReader.setScaledSize(label->size());
    if (!imageReader.canRead())
    {
        return;
    }
    QImage image = imageReader.read();
    QPixmap pixmap = QPixmap::fromImage(image);

    // set a scaled pixmap to a w x h window keeping its aspect ratio
    label->setPixmap(pixmap);
}

void FullMatchResultForm::contentChanged()
{
    QString teamA, teamB;
    teamA = statusModel->getTeamAName();
    teamB = statusModel->getTeamBName();

    ui->teamANameLabel->setText(teamA);
    ui->teamBNameLabel->setText(teamB);
    int teamAResult, teamBResult;

    statusModel->getCurrentResult(teamAResult, teamBResult);

    ui->teamAPointsLabel->setText(QString::number(teamAResult));
    ui->teamBPointsLabel->setText(QString::number(teamBResult));

    drawImage(ui->teamALogoLabel, statusModel->getTeamALogoFile());
    drawImage(ui->teamBLogoLabel, statusModel->getTeamBLogoFile());

    int currentGame, currentMatch;
    int playerAPoint, playerBPoint;
    int playerAGamePoint, playerBGamePoint;

    QString playerAName, playerBName;

    statusModel->getCurrentMatch(currentGame, currentMatch);

    statusModel->getPoints(currentGame, currentMatch, playerAPoint, playerBPoint);
    statusModel->getPlayerName(currentGame, playerAName, playerBName);

    ui->playerAPointLabel->setText(QString::number(playerAPoint));
    ui->playerBPointLabel->setText(QString::number(playerBPoint));
    ui->playerANameLabel->setText(" " + playerAName);
    ui->playerBNameLabel->setText(playerBName + " ");

    statusModel->getCurrentGameResult(currentGame, playerAGamePoint, playerBGamePoint);

    ui->playerAGamePointLabel->setText(QString::number(playerAGamePoint));
    ui->playerBGamePointLabel->setText(QString::number(playerBGamePoint));

    QDate today = QDate::currentDate();
    ui->dayLabel->setText(today.toString("dd/MM/yyyy"));
}
