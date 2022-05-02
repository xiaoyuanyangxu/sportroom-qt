#include "currentmatchresultform.h"
#include "sportroomutils.h"
#include "ui_currentmatchresultform.h"

#include <QSettings>

CurrentMatchResultForm::CurrentMatchResultForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CurrentMatchResultForm),
    matchModel(0),
    stateModel(0),
    wantSummaryMode(false)
{
    ui->setupUi(this);
    int nameSize = (this->size().width() - 40)/2;
    int totalWidth = this->size().width() - 40;
    int totalHeight = this->size().height();
    ui->tableWidget->setColumnWidth(0, nameSize);
    for (int i=1 ; i<=6 ; i++)
    {
        ui->tableWidget->setColumnWidth(i, (totalWidth - nameSize)/6);
    }
    ui->tableWidget->setColumnWidth(1, (totalWidth - nameSize)/6/2);
    ui->tableWidget->setRowHeight(0, totalHeight/2);
    ui->tableWidget->setRowHeight(1, totalHeight/2);
}

CurrentMatchResultForm::~CurrentMatchResultForm()
{
    delete ui;
}

void CurrentMatchResultForm::setModels(MatchStatus * matchModel, StateDatamodel * stateModel, bool wantSummaryMode)
{
    this->matchModel = matchModel;
    this->stateModel = stateModel;
    this->wantSummaryMode = wantSummaryMode;

    QObject::connect(this->matchModel, &MatchStatus::contentChanged,
                     this, &CurrentMatchResultForm::contentChanged);

    QObject::connect(this->stateModel, &StateDatamodel::contentChanged,
                     this, &CurrentMatchResultForm::contentChanged);

    contentChanged();
}

void CurrentMatchResultForm::fullMode(QTableWidgetItem * item, int currentMatch, int currentGame)
{
    QSettings settings;

    QColor primaryColorBack = QColor(settings.value("primary_back","#FFFFFF").toString());
    QColor primaryColorText = QColor(settings.value("primary_text","#000000").toString());

    QColor secondaryColorBack = QColor(settings.value("secondary_back","#FFFFFF").toString());
    QColor secondaryColorText = QColor(settings.value("secondary_text","#000000").toString());

    int maxMatch = -1;
    for (int i = 0; i<5 ; i++)
    {
        int playerA, playerB;
        matchModel->getPoints(currentMatch, i, playerA, playerB);
        if ((playerA > 0 || playerB > 0) && i > maxMatch) {
            maxMatch = i;
        }
        if (playerA == 0 && playerB == 0 && currentGame != i)
        {
            ui->tableWidget->setItem(0, i+2, new QTableWidgetItem(""));
            ui->tableWidget->setItem(1, i+2, new QTableWidgetItem(""));
        }else{
            item = new QTableWidgetItem(QString::number(playerA));
            item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            item->setTextColor((i%2==0)?secondaryColorText:primaryColorText);
            item->setBackgroundColor((i%2==0)?secondaryColorBack:primaryColorBack);
            ui->tableWidget->setItem(0, i+2, item);

            item = new QTableWidgetItem(QString::number(playerB));
            item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            item->setTextColor((i%2==0)?secondaryColorText:primaryColorText);
            item->setBackgroundColor((i%2==0)?secondaryColorBack:primaryColorBack);
            ui->tableWidget->setItem(1, i+2, item);
        }
    }

    int totalSize = this->size().width() - 40;
    if (stateModel->getElementState(0x10)) // lazy mode
    {
       if (maxMatch >= 0){
           int playerA, playerB;
           matchModel->getPoints(currentMatch, maxMatch, playerA, playerB);
           if ( playerA < 11 && playerB < 11 ) {
                maxMatch --;
           }else if (std::abs(playerA - playerB) < 2) {
                maxMatch --;
           }

       }
    }
    ui->tableWidget->setMaximumWidth(totalSize / 2 + (maxMatch+1.99) * (totalSize / 2/6));
    ui->tableWidget->setMinimumWidth(totalSize / 2 + (maxMatch+1.99) * (totalSize / 2/6));
}

void CurrentMatchResultForm::summaryMode(QTableWidgetItem *item, int currentMatch, int currentGame)
{
    QSettings settings;

    QColor primaryColorBack = QColor(settings.value("primary_back","#FFFFFF").toString());
    QColor primaryColorText = QColor(settings.value("primary_text","#000000").toString());

    QColor secondaryColorBack = QColor(settings.value("secondary_back","#FFFFFF").toString());
    QColor secondaryColorText = QColor(settings.value("secondary_text","#000000").toString());

    int maxMatch = -1;
    int playerA, playerB;
    int currentMatchA, currentMatchB;

    // Current Match
    maxMatch = matchModel->getCurrentGameResult(currentMatch, playerA, playerB);

    item = new QTableWidgetItem(QString::number(playerA));
    item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    item->setTextColor(secondaryColorText);
    item->setBackgroundColor(secondaryColorBack);
    ui->tableWidget->setItem(0, 2, item);

    item = new QTableWidgetItem(QString::number(playerB));
    item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    item->setTextColor(secondaryColorText);
    item->setBackgroundColor(secondaryColorBack);

    ui->tableWidget->setItem(1, 2, item);

    // Current Game
    matchModel->getPoints(currentMatch, currentGame, currentMatchA, currentMatchB);
    item = new QTableWidgetItem(QString::number(currentMatchA));
    item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    item->setTextColor(primaryColorText);
    item->setBackgroundColor(primaryColorBack);

    ui->tableWidget->setItem(0, 3, item);

    item = new QTableWidgetItem(QString::number(currentMatchB));
    item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    item->setTextColor(primaryColorText);
    item->setBackgroundColor(primaryColorBack);
    ui->tableWidget->setItem(1, 3, item);



    int totalSize = this->size().width() - 40;
    if (playerA == 0 && playerB == 0 && currentMatchA == 0 && currentMatchB == 0 && stateModel->getElementState(0x10))
    {
        ui->tableWidget->setMaximumWidth(totalSize / 2);
        ui->tableWidget->setMinimumWidth(totalSize / 2);
    }else{
        ui->tableWidget->setMaximumWidth(totalSize / 2 + (2.99) * (totalSize/2/6));
        ui->tableWidget->setMinimumWidth(totalSize / 2 + (2.99) * (totalSize/2/6));
    }
}

void CurrentMatchResultForm::serveBallVisibility(int currentMatch, int currentGame, bool &ballAVisible, bool &ballBVisible)
{
    int playerA, playerB;
    bool startPlayerA=false;
    matchModel->getPoints(currentMatch, currentGame, playerA, playerB);
    if (matchModel->getPlayerAServe(currentMatch)){
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
    if (!startPlayerA){
        servePlayerA = !servePlayerA;
    }
    ballAVisible = false;
    ballBVisible = false;
    bool endGame = false;
    if (playerA > 10 || playerB>10){
        if (std::abs(playerA - playerB) > 1){
            endGame = true;
        }
    }
    bool visible = stateModel->getElementState(0x08);
    if (!endGame && visible) {
       ballAVisible = servePlayerA;
       ballBVisible = !servePlayerA;
    }

    //ui->playerAServeLabel->setVisible(ballAVisible);
    //ui->playerBServeLabel->setVisible(ballBVisible);
}

void CurrentMatchResultForm::contentChanged()
{
    QTableWidgetItem * item;
    int currentMatch, currentGame;
    QSettings settings;

    QColor primaryColorBack = QColor(settings.value("primary_back","#FFFFFF").toString());
    QColor primaryColorText = QColor(settings.value("primary_text","#000000").toString());

    //QColor color1(0,63,114);
    //QColor color2(218,229,237);

    QString playerAName, playerBName;
    matchModel->getCurrentMatch(currentMatch, currentGame);
    matchModel->getPlayerName(currentMatch, playerAName, playerBName);

    item = new QTableWidgetItem("" + playerAName + "");
    item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    item->setTextColor(primaryColorText);
    item->setBackgroundColor(primaryColorBack);
    item->setIcon(SportRoomUtils::SvgToQIcon(":/images/t-shirt.svg",
                                             matchModel->getTeamAColor(),
                                             SportRoomUtils::contrastColor(matchModel->getTeamAColor())));
    ui->tableWidget->setItem(0,0, item);

    item = new QTableWidgetItem(""  + playerBName + "");
    item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    item->setTextColor(primaryColorText);
    item->setBackgroundColor(primaryColorBack);
    item->setIcon(SportRoomUtils::SvgToQIcon(":/images/t-shirt.svg",
                                             matchModel->getTeamBColor(),
                                             SportRoomUtils::contrastColor(matchModel->getTeamBColor())));
    ui->tableWidget->setItem(1,0, item);


    ui->playerATimeoutLabel->setVisible(matchModel->getPlayerATimeout(currentMatch));
    ui->playerBTimeoutLabel->setVisible(matchModel->getPlayerBTimeout(currentMatch));

    // Serve ball
    bool aVisible, bVisible;
    serveBallVisibility(currentMatch, currentGame, aVisible, bVisible);

    item = new QTableWidgetItem();
    item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    item->setTextColor(primaryColorText);
    item->setBackgroundColor(primaryColorBack);
    if (aVisible) {
        item->setIcon(SportRoomUtils::SvgToQIcon(":/images/triangle_left.svg",
                                             settings.value("primary_text","#FFFFFF").toString(),
                                             settings.value("primary_text","#FFFFFF").toString()));
    }

    ui->tableWidget->setItem(0, 1, item);

    item = new QTableWidgetItem();
    item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    item->setTextColor(primaryColorText);
    item->setBackgroundColor(primaryColorBack);
    if (bVisible) {
        item->setIcon(SportRoomUtils::SvgToQIcon(":/images/triangle_left.svg",
                                             settings.value("primary_text","#FFFFFF").toString(),
                                             settings.value("primary_text","#FFFFFF").toString()));
    }

    ui->tableWidget->setItem(1, 1, item);

    if (wantSummaryMode){
        summaryMode(item, currentMatch, currentGame);
    }else{
        fullMode(item, currentMatch, currentGame);
    }


}
