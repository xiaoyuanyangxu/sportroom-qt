#include "scoreboardform.h"
#include "ui_scoreboardform.h"

ScoreboardForm::ScoreboardForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ScoreboardForm)
{
    ui->setupUi(this);
}

ScoreboardForm::~ScoreboardForm()
{
    delete ui;
}

void ScoreboardForm::setStatusModel(MatchStatus *statusModel)
{
    this->statusModel = statusModel;

    QObject::connect(statusModel, &MatchStatus::contentChanged,
                     this, &ScoreboardForm::contentChanged);

    contentChanged();

}

void ScoreboardForm::contentChanged()
{
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

    bool playerATimeout, playerBTimeout;
    playerATimeout = statusModel->getPlayerATimeout(currentGame);
    playerBTimeout = statusModel->getPlayerBTimeout(currentGame);

    ui->playerATimeOutLabel->setStyleSheet(playerATimeout?"color: rgb(232, 23, 150);\nbackground-color: rgb(255, 255, 255);":"background-color: rgb(0, 0, 0);");
    ui->playerBTimeOutLabel->setStyleSheet(playerBTimeout?"color: rgb(232, 23, 150);\nbackground-color: rgb(255, 255, 255);":"background-color: rgb(0, 0, 0);");

    ui->playerATimeOutLabel->setText(playerATimeout?"T":"");
    ui->playerBTimeOutLabel->setText(playerBTimeout?"T":"");
}
