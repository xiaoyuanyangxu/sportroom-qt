#include "currentmatchresultform.h"
#include "ui_currentmatchresultform.h"

CurrentMatchResultForm::CurrentMatchResultForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CurrentMatchResultForm),
    statusModel(0)
{
    ui->setupUi(this);
    int nameSize = (this->size().width() - 40 - 16)/2 - 5;
    int totalWidth = this->size().width() - 40 -16;
    int totalHeight = this->size().height();
    ui->tableWidget->setColumnWidth(0, nameSize);
    for (int i=1 ; i<=5 ; i++)
    {
        ui->tableWidget->setColumnWidth(i, (totalWidth - nameSize)/5);
    }
    ui->tableWidget->setRowHeight(0, totalHeight/2);
    ui->tableWidget->setRowHeight(1, totalHeight/2);
}

CurrentMatchResultForm::~CurrentMatchResultForm()
{
    delete ui;
}

void CurrentMatchResultForm::setStatusModel(MatchStatus *statusModel)
{
    this->statusModel = statusModel;

    QObject::connect(statusModel, &MatchStatus::contentChanged,
                     this, &CurrentMatchResultForm::contentChanged);
    contentChanged();
}

void CurrentMatchResultForm::contentChanged()
{
    QTableWidgetItem * item;
    int currentMath, currentGame;
    QColor color1(0,63,114);
    QColor color2(218,229,237);

    QString playerAName, playerBName;
    statusModel->getCurrentMatch(currentMath, currentGame);
    statusModel->getPlayerName(currentMath, playerAName, playerBName);

    item = new QTableWidgetItem("  " + playerAName + "");
    item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    item->setTextColor(color1);
    ui->tableWidget->setItem(0,0, item);
    item = new QTableWidgetItem("  "  + playerBName + "");
    item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    ui->tableWidget->setItem(1,0, item);
    item->setTextColor(color1);

    ui->playerATimeoutLabel->setVisible(statusModel->getPlayerATimeout(currentMath));
    ui->playerBTimeoutLabel->setVisible(statusModel->getPlayerBTimeout(currentMath));

    for (int i = 0; i<5 ; i++)
    {
        int playerA, playerB;
        statusModel->getPoints(currentMath, i, playerA, playerB);
        if (playerA == 0 && playerB == 0 && currentGame != i)
        {
            ui->tableWidget->setItem(0, i+1, new QTableWidgetItem(""));
            ui->tableWidget->setItem(1, i+1, new QTableWidgetItem(""));
        }else{
            item = new QTableWidgetItem(QString::number(playerA));
            item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            item->setTextColor((i%2==0)?color2:color1);
            ui->tableWidget->setItem(0, i+1, item);

            item = new QTableWidgetItem(QString::number(playerB));
            item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            item->setTextColor((i%2==0)?color2:color1);
            ui->tableWidget->setItem(1, i+1, item);
        }
    }

    int playerA, playerB;
    bool startPlayerA=false;
    statusModel->getPoints(currentMath, currentGame, playerA, playerB);
    if (statusModel->getPlayerAServe(currentMath)){
        if ((currentGame%2) == 0){
            startPlayerA = true;
        }
    }else{
        if ((currentGame%2) == 1){
            startPlayerA = true;
        }
    }
    bool servePlayerA = false;
    int totalPoints = playerA + playerB;
    if (playerA < 10 || playerB < 10)
    {
        if ((totalPoints % 4) == 0 || (totalPoints % 4) == 1){
            servePlayerA = true;
        }
    }else{
        if (((totalPoints-20) % 2) == 0){
            servePlayerA = true;
        }
    }
    if (startPlayerA){
        servePlayerA = !servePlayerA;
    }
    bool ballAVisible = false;
    bool ballBVisible = false;
    bool endGame = false;
    if (playerA > 10 || playerB>10){
        if (std::abs(playerA - playerB) > 1){
            endGame = true;
        }
    }
    if (!endGame) {
       ballAVisible = servePlayerA;
       ballBVisible = !servePlayerA;
    }
    ui->playerAServeLabel->setVisible(ballAVisible);
    ui->playerBServeLabel->setVisible(ballBVisible);
}
