#include "playerstatsform.h"
#include "ui_playerstatsform.h"

PlayerStatsForm::PlayerStatsForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PlayerStatsForm)
{
    ui->setupUi(this);
}

PlayerStatsForm::~PlayerStatsForm()
{
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

void PlayerStatsForm::playerContentChanged()
{
    PlayerStat stats;
    players->getPlayerStats(playerAName, stats);
    ui->playerANameLabel->setText(stats.name);
    ui->playerAGamePlayedLabel->setText(QString::number(stats.gamePlayed));
    ui->playerAWinGamesLabel->setText(QString("%1(%2%)").arg(stats.winGames).arg(float(stats.winGames)*100/float(stats.gamePlayed), 2,'f', 0));
    ui->playerAPointPlayedLabel->setText(QString("%1(%2)").arg(stats.pointPlayed).arg(stats.winPoints-(stats.pointPlayed-stats.winPoints)));
    ui->playerAWinPointsLabel->setText(QString::number(stats.winPoints));
    ui->playerALostPointsLabel->setText(QString::number(stats.pointPlayed - stats.winPoints));
    ui->playerAMatchPlayedLabel->setText(QString("%1(%2)").arg(stats.matchPlayed).arg(stats.winMatch-(stats.matchPlayed-stats.winMatch)));
    ui->playerAWinMatchesLabel->setText(QString::number(stats.winMatch));
    ui->playerALostMatchesLabel->setText(QString::number(stats.matchPlayed - stats.winMatch));

    players->getPlayerStats(playerBName, stats);
    ui->playerBNameLabel->setText(stats.name);
    ui->playerBGamePlayedLabel->setText(QString::number(stats.gamePlayed));
    ui->playerBWinGamesLabel->setText(QString("%1(%2%)").arg(stats.winGames).arg(float(stats.winGames)*100/float(stats.gamePlayed), 2,'f', 0));
    ui->playerBPointPlayedLabel->setText(QString("%1(%2)").arg(stats.pointPlayed).arg(stats.winPoints-(stats.pointPlayed-stats.winPoints)));
    ui->playerBWinPointsLabel->setText(QString::number(stats.winPoints));
    ui->playerBLostPointsLabel->setText(QString::number(stats.pointPlayed - stats.winPoints));
    ui->playerBMatchPlayedLabel->setText(QString("%1(%2)").arg(stats.matchPlayed).arg(stats.winMatch-(stats.matchPlayed-stats.winMatch)));
    ui->playerBWinMatchesLabel->setText(QString::number(stats.winMatch));
    ui->playerBLostMatchesLabel->setText(QString::number(stats.matchPlayed - stats.winMatch));
}
