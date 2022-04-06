#include "currentmatchresultform.h"
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
            ui->tableWidget->setItem(0, i+1, new QTableWidgetItem(""));
            ui->tableWidget->setItem(1, i+1, new QTableWidgetItem(""));
        }else{
            item = new QTableWidgetItem(QString::number(playerA));
            item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            item->setTextColor((i%2==0)?secondaryColorText:primaryColorText);
            item->setBackgroundColor((i%2==0)?secondaryColorBack:primaryColorBack);
            ui->tableWidget->setItem(0, i+1, item);

            item = new QTableWidgetItem(QString::number(playerB));
            item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            item->setTextColor((i%2==0)?secondaryColorText:primaryColorText);
            item->setBackgroundColor((i%2==0)?secondaryColorBack:primaryColorBack);
            ui->tableWidget->setItem(1, i+1, item);
        }
    }

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
    ui->tableWidget->setMaximumWidth(469 / 2 + (maxMatch+1) * (469/2/5));
    ui->tableWidget->setMinimumWidth(469 / 2 + (maxMatch+1) * (469/2/5));
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

    ui->tableWidget->setItem(0, 1, item);

    item = new QTableWidgetItem(QString::number(playerB));
    item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    item->setTextColor(secondaryColorText);
    item->setBackgroundColor(secondaryColorBack);

    ui->tableWidget->setItem(1, 1, item);

    // Current Game
    matchModel->getPoints(currentMatch, currentGame, currentMatchA, currentMatchB);
    item = new QTableWidgetItem(QString::number(currentMatchA));
    item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    item->setTextColor(primaryColorText);
    item->setBackgroundColor(primaryColorBack);

    ui->tableWidget->setItem(0, 2, item);

    item = new QTableWidgetItem(QString::number(currentMatchB));
    item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    item->setTextColor(primaryColorText);
    item->setBackgroundColor(primaryColorBack);
    ui->tableWidget->setItem(1, 2, item);

    if (playerA == 0 && playerB == 0 && currentMatchA == 0 && currentMatchB == 0 && stateModel->getElementState(0x10))
    {
        ui->tableWidget->setMaximumWidth(469 / 2);
        ui->tableWidget->setMinimumWidth(469 / 2);
    }else{
        ui->tableWidget->setMaximumWidth(469 / 2 + (2) * (469/2/5));
        ui->tableWidget->setMinimumWidth(469 / 2 + (2) * (469/2/5));
    }
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

    item = new QTableWidgetItem("  " + playerAName + "");
    item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    item->setTextColor(primaryColorText);
    item->setBackgroundColor(primaryColorBack);

    ui->tableWidget->setItem(0,0, item);
    item = new QTableWidgetItem("  "  + playerBName + "");
    item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    ui->tableWidget->setItem(1,0, item);
    item->setTextColor(primaryColorText);
    item->setBackgroundColor(primaryColorBack);
    ui->playerATimeoutLabel->setVisible(matchModel->getPlayerATimeout(currentMatch));
    ui->playerBTimeoutLabel->setVisible(matchModel->getPlayerBTimeout(currentMatch));

    if (wantSummaryMode){
        summaryMode(item, currentMatch, currentGame);
    }else{
        fullMode(item, currentMatch, currentGame);
    }

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
    bool ballAVisible = false;
    bool ballBVisible = false;
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
    ui->playerAServeLabel->setVisible(ballAVisible);
    ui->playerBServeLabel->setVisible(ballBVisible);
}
