#include "playerstatsform.h"
#include "ui_playerstatsform.h"

#include <QFileDialog>
#include <QDebug>
#include <QContextMenuEvent>

#include "sportroomutils.h"

PlayerStatsForm::PlayerStatsForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PlayerStatsForm),
    contextMenu(0)
{
    ui->setupUi(this);
}

PlayerStatsForm::~PlayerStatsForm()
{
    delete contextMenu;
    delete ui;
}

void PlayerStatsForm::setModels(MatchStatus *status, PlayerDatamodel *players)
{
    this->status = status;
    this->players = players;

    QObject::connect(this->status, &MatchStatus::contentChanged,
                     this, &PlayerStatsForm::statusContentChanged);


    QObject::connect(this->players, &PlayerDatamodel::contentChanged,
                     this, &PlayerStatsForm::playerContentChanged);

    statusContentChanged();
}

void PlayerStatsForm::statusContentChanged()
{
    int game, match;
    status->getCurrentMatch(game, match);
    status->getPlayerName(game, playerAName, playerBName);

    playerContentChanged();
}

void PlayerStatsForm::createContextMenu()
{
    delete(contextMenu);

    contextMenu  = new QMenu(this);

    contextMenu->setStyleSheet("color: black;background-color: white;");

    QAction *changePlayerAImage;
    changePlayerAImage   = new QAction(QObject::tr("%1 Image").arg(playerAName),
                             this);
    connect(changePlayerAImage,
            SIGNAL(triggered()),
            this,
            SLOT(changePlayerAImageSlot()));

    QAction *changePlayerBImage;
    changePlayerBImage   = new QAction(QObject::tr("%1 Image").arg(playerBName),
                             this);
    connect(changePlayerBImage,
            SIGNAL(triggered()),
            this,
            SLOT(changePlayerBImageSlot()));


    contextMenu->addAction(changePlayerAImage);
    contextMenu->addAction(changePlayerBImage);

}

void PlayerStatsForm::changePlayerAImageSlot()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                   "Select Image File",
                                                   "",
                                                   tr("Images (*.png *.png)"));
    if (!fileName.isEmpty())
    {
        players->setPlayerImagePath(playerAName, fileName);
    }
}

void PlayerStatsForm::changePlayerBImageSlot()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                   "Select Image File",
                                                   "",
                                                   tr("Images (*.png *.png)"));
    if (!fileName.isEmpty())
    {
        players->setPlayerImagePath(playerBName, fileName);
    }
}

void PlayerStatsForm::playerContentChanged()
{
    PlayerStat stats;
    QString path = ":/images/person.png";

    players->getPlayerStats(playerAName, stats);
    ui->playerANameLabel->setText(SportRoomUtils::toCamelCase(stats.name));

    ui->playerAPercentLabel->setText(QString("%1%").arg(float(stats.winGames)*100/float(stats.gamePlayed), 2,'f', 0));

    ui->playerAGamePlayedLabel->setText(QString::number(stats.gamePlayed));
    ui->playerAWinGamesLabel->setText(QString("%1").arg(stats.winGames));

    ui->playerAPointPlayedLabel->setText(QString("%1(%2)").arg(stats.pointPlayed).arg(stats.winPoints-(stats.pointPlayed-stats.winPoints)));
    ui->playerAWinPointsLabel->setText(QString::number(stats.winPoints));
    ui->playerALostPointsLabel->setText(QString::number(stats.pointPlayed - stats.winPoints));
    ui->playerAMatchPlayedLabel->setText(QString("%1(%2)").arg(stats.matchPlayed).arg(stats.winMatch-(stats.matchPlayed-stats.winMatch)));
    ui->playerAWinMatchesLabel->setText(QString::number(stats.winMatch));
    ui->playerALostMatchesLabel->setText(QString::number(stats.matchPlayed - stats.winMatch));

    ui->playerANameHeaderLabel->setText(stats.name);

    SportRoomUtils::drawImage(ui->playerAImageLabel, (stats.imagePath.isEmpty())?path:stats.imagePath);
    SportRoomUtils::drawImage(ui->teamALogoLabel,this->status->getTeamALogoFile());

    players->getPlayerStats(playerBName, stats);
    ui->playerBNameLabel->setText(SportRoomUtils::toCamelCase(stats.name));
    ui->playerBPercentLabel->setText(QString("%1%").arg(float(stats.winGames)*100/float(stats.gamePlayed), 2,'f', 0));

    ui->playerBGamePlayedLabel->setText(QString::number(stats.gamePlayed));
    ui->playerBWinGamesLabel->setText(QString("%1").arg(stats.winGames));

    ui->playerBPointPlayedLabel->setText(QString("%1(%2)").arg(stats.pointPlayed).arg(stats.winPoints-(stats.pointPlayed-stats.winPoints)));
    ui->playerBWinPointsLabel->setText(QString::number(stats.winPoints));
    ui->playerBLostPointsLabel->setText(QString::number(stats.pointPlayed - stats.winPoints));
    ui->playerBMatchPlayedLabel->setText(QString("%1(%2)").arg(stats.matchPlayed).arg(stats.winMatch-(stats.matchPlayed-stats.winMatch)));
    ui->playerBWinMatchesLabel->setText(QString::number(stats.winMatch));
    ui->playerBLostMatchesLabel->setText(QString::number(stats.matchPlayed - stats.winMatch));

    ui->playerBNameHeaderLabel->setText(stats.name);

    SportRoomUtils::drawImage(ui->playerBImageLabel, (stats.imagePath.isEmpty())?path:stats.imagePath);
    SportRoomUtils::drawImage(ui->teamBLogoLabel,this->status->getTeamBLogoFile());
}

void PlayerStatsForm::contextMenuEvent(QContextMenuEvent *event)
{
    qDebug()<< Q_FUNC_INFO<<event;
    createContextMenu();
    contextMenu->popup(event->globalPos());
}

void PlayerStatsForm::resizeEvent(QResizeEvent *event)
{
    playerContentChanged();
}
