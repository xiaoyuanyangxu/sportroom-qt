#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "currentmatchresultdialog.h"
#include "teamresultdialog.h"
#include "fullmatchresultdialog.h"

#include <QCompleter>
#include <QDate>
#include <QFileDialog>
#include <QFileDialog>
#include <QHeaderView>
#include <QImageReader>
#include <QInputDialog>
#include <QMessageBox>
#include <QMessageBox>
#include <QMessageBox>
#include <QSettings>
#include <QDebug>

#include "sportroomutils.h"
#include "playerandteamsdialog.h"
#include "selectplayerdialog.h"
#include "playerstatsdialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    initState();
    updateData();
    initializeResultTable();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateData()
{
    matchStatusModel->getPoints(currentMatch, currentGame, playerAPoint, playerBPoint);
    matchStatusModel->getPlayerName(currentMatch, playerAName, playerBName);
    ui->teamANameLineEdit->setText(matchStatusModel->getTeamAName());
    ui->teamBNameLineEdit->setText(matchStatusModel->getTeamBName());

    SportRoomUtils::drawImage(ui->teamBLogoLabel, matchStatusModel->getTeamBLogoFile());
    SportRoomUtils::drawImage(ui->teamALogoLabel, matchStatusModel->getTeamALogoFile());

    QSettings settings;

    if(settings.value("selected_language").toString() == "es_ES")
    {
        ui->spanishToolButton->setChecked(true);
    }else{
        ui->spanishToolButton->setChecked(false);
    }
    if(settings.value("selected_language").toString() == "ca_ES")
    {
        ui->catalaToolButton->setChecked(true);
    }else{
        ui->catalaToolButton->setChecked(false);
    }
    if((settings.value("selected_language").toString() == "en_EN") || (settings.value("selected_language").toString().isEmpty()))
    {
        ui->englishToolButton->setChecked(true);
    }else{
        ui->englishToolButton->setChecked(false);
    }
}

void MainWindow::initState()
{
    playerAName = "";
    playerBName = "";
    playerAPoint = 0;
    playerBPoint = 0;

    currentGame  = 0;
    currentMatch = 0;

    matchStatusModel = new MatchStatus();
    QObject::connect(matchStatusModel, &MatchStatus::contentChanged,
                     this, &MainWindow::contentChanged);

    ui->scoreboardWidget->setStatusModel(matchStatusModel);

    matchStatusModel->readStatus();

    playerModel = new PlayerDatamodel();

    QObject::connect(playerModel, &PlayerDatamodel::contentChanged,
                     this, &MainWindow::playerContentChanged);

    playerContentChanged();

}


void MainWindow::on_currentMatchResultPushButton_clicked()
{
    CurrentMatchResultDialog *dialog = new CurrentMatchResultDialog(matchStatusModel, this);

    dialog->show();

    connect(dialog,
            &CurrentMatchResultDialog::finished,
            [=](int result){
                Q_UNUSED(result);
                dialog->hide();
                dialog->deleteLater();
    });
}

void MainWindow::initializeResultTable()
{
    ui->matchResultTableView->setModel(matchStatusModel);


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

    //ui->deliveryTableView->setItemDelegate(new DeliveryTableItemDelegate(ui->deliveryTableView));
}

void MainWindow::on_matchResultTableView_doubleClicked(const QModelIndex &index)
{
    if (index.column() == 1 || index.column() == 3)
    {
        bool ok = true;
        QString aName, bName;

        matchStatusModel->getPlayerName(index.row(), aName, bName);

        SelectPlayerDialog * dialog = new SelectPlayerDialog(playerModel, this);

        dialog->setModal(true);

        dialog->show();

        connect(dialog,
                &SelectPlayerDialog::finished,
                [=](int result){
                    if (result) {
                        QString text = dialog->getPlayerName();

                        if (ok && !text.isEmpty()) {
                            if (index.column() == 1){
                                matchStatusModel->setPlayerAName(index.row(), text);
                            }else{
                                matchStatusModel->setPlayerBName(index.row(), text);
                            }

                            if (index.row() >= 0 && index.row() <=2)
                            {
                                if (index.column() == 1 )
                                {
                                    int otherGame[3] = {3,5,4};
                                    matchStatusModel->setPlayerAName(otherGame[index.row()], text);
                                }else{
                                    int otherGame[3] = {4,3,5};
                                    matchStatusModel->setPlayerBName(otherGame[index.row()], text);
                                }
                            }

                        }
                    }
                    dialog->hide();
                    dialog->deleteLater();
        });
    } else if (index.column() >= 4 && index.column()<=8)
    {
        currentMatch = index.row();
        currentGame = index.column() - 4;

        matchStatusModel->setCurrentMatch(currentMatch, currentGame);
        matchStatusModel->setPoints(currentMatch, currentGame, 0, 0);
        //updateData();
    }
}

void MainWindow::on_playerAUpPushButton_clicked()
{
    playerAPoint ++;
    matchStatusModel->setPoints(currentMatch, currentGame, playerAPoint, playerBPoint);
}

void MainWindow::on_playerADownPushButton_clicked()
{
    playerAPoint  = std::max(playerAPoint-1, 0);

    matchStatusModel->setPoints(currentMatch, currentGame, playerAPoint, playerBPoint);
}

void MainWindow::on_playerAResetPushButton_clicked()
{
    playerAPoint = 0 ;
    matchStatusModel->setPoints(currentMatch, currentGame, playerAPoint, playerBPoint);
}

void MainWindow::on_playerBUpPushButton_clicked()
{
    playerBPoint ++;
    matchStatusModel->setPoints(currentMatch, currentGame, playerAPoint, playerBPoint);
}

void MainWindow::on_playerBDownPushButton_clicked()
{
    playerBPoint  = std::max(playerBPoint-1, 0);
    matchStatusModel->setPoints(currentMatch, currentGame, playerAPoint, playerBPoint);
}

void MainWindow::on_playerBResetPushButton_clicked()
{
    playerBPoint = 0;
    matchStatusModel->setPoints(currentMatch, currentGame, playerAPoint, playerBPoint);
}



void MainWindow::on_matchResultTableView_clicked(const QModelIndex &index)
{
    if (index.column() >= 4 && index.column()<=8)
    {
        currentMatch = index.row();
        currentGame = index.column() - 4;

        matchStatusModel->getPoints(currentMatch, currentGame,  playerAPoint, playerBPoint);
        matchStatusModel->getPlayerName(currentMatch, playerAName, playerBName);
        matchStatusModel->setCurrentMatch(currentMatch, currentGame);
        //updateData();
    }
}

void MainWindow::on_teamANameLineEdit_editingFinished()
{
    matchStatusModel->setTeamAName(ui->teamANameLineEdit->text());
}

void MainWindow::on_teamBNameLineEdit_editingFinished()
{
    matchStatusModel->setTeamBName(ui->teamBNameLineEdit->text());
}

void MainWindow::on_newGamePushButton_clicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, tr("Confirm"), tr("Can you confirm to Reset the Current Match?"),
                                QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        matchStatusModel->reset();
    }
}

void MainWindow::contentChanged()
{
    updateData();
}

void MainWindow::playerContentChanged()
{
    QStringList allTeams;
    playerModel->getTeamNameList(allTeams);

    QCompleter *completer = new QCompleter(allTeams, this);

    completer->setCaseSensitivity(Qt::CaseInsensitive);

    ui->teamANameLineEdit->setCompleter(completer);
    ui->teamBNameLineEdit->setCompleter(completer);
}

void MainWindow::on_teamResultPushButton_clicked()
{
    TeamResultDialog *dialog = new TeamResultDialog(matchStatusModel, this);

    dialog->show();

    connect(dialog,
            &TeamResultDialog::finished,
            [=](int result){
                Q_UNUSED(result);
                dialog->hide();
                dialog->deleteLater();
    });
}

void MainWindow::on_teamALogoPushButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                   "Select Image File",
                                                   "",
                                                   tr("Images (*.png *.png)"));

    if (!fileName.isEmpty())
    {
        matchStatusModel->setTeamALogoFile(fileName);
        //drawImage(ui->teamALogoLabel, fileName);
    }
}

void MainWindow::on_teamBLogoPushButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                   "Select Image File",
                                                   "",
                                                   tr("Images (*.png *.png)"));

    if (!fileName.isEmpty())
    {
        matchStatusModel->setTeamBLogoFile(fileName);
        //SportRoomUtils::drawImage(ui->teamBLogoLabel, fileName);
    }
}

void MainWindow::on_exchangePushButton_clicked()
{
    matchStatusModel->exchangeTeam();
}

void MainWindow::on_playerATimeoutPushButton_clicked()
{
    bool timeout = matchStatusModel->getPlayerATimeout(currentGame);
    matchStatusModel->setPlayerATimeout(currentGame, !timeout);
}

void MainWindow::on_playerBTimeoutPushButton_clicked()
{
    bool timeout = matchStatusModel->getPlayerBTimeout(currentGame);
    matchStatusModel->setPlayerBTimeout(currentGame, !timeout);
}

void MainWindow::on_fullResultPushButton_clicked()
{
    FullMatchResultDialog *dialog = new FullMatchResultDialog(matchStatusModel, this);

    SportRoomUtils::recoverSize(dialog, "full_match_result");
    dialog->setWindowFlags(Qt::Window);
    dialog->show();


    connect(dialog,
            &FullMatchResultDialog::finished,
            [=](int result){
                Q_UNUSED(result);
                dialog->hide();
                dialog->deleteLater();
    });
}


void MainWindow::on_playerTeamPushButton_clicked()
{
    PlayerAndTeamsDialog *dialog = new PlayerAndTeamsDialog(playerModel, this);
    SportRoomUtils::recoverSize(dialog, "player_and_teams");
    dialog->setWindowFlags(Qt::Window);
    dialog->show();


    connect(dialog,
            &FullMatchResultDialog::finished,
            [=](int result){
                Q_UNUSED(result);
                dialog->hide();
                dialog->deleteLater();
    });
}

void MainWindow::on_playerStatsPushButton_clicked()
{
    PlayerStatsDialog *dialog = new PlayerStatsDialog(matchStatusModel, playerModel, this);

    SportRoomUtils::recoverSize(dialog, "player_stats");
    dialog->setWindowFlags(Qt::Window);
    dialog->show();

    connect(dialog,
            &FullMatchResultDialog::finished,
            [=](int result){
                Q_UNUSED(result);
                dialog->hide();
                dialog->deleteLater();
    });
}


void MainWindow::on_exportToolButton_clicked()
{
    QString teamAName, teamBName;

    teamAName = matchStatusModel->getTeamAName().replace(" ","_");
    teamBName = matchStatusModel->getTeamBName().replace(" ","_");

    QString path = QString("sport_room_%1_%2_%3.json").arg(teamAName).arg(teamBName).arg(QDateTime::currentSecsSinceEpoch());

    QString fileName = QFileDialog::getSaveFileName(this,
                                                   "Select Json File",
                                                   path,
                                                   tr("Json (*.json)"));
    if (!fileName.isEmpty())
    {
        matchStatusModel->exportInfo(fileName);
    }
}

void MainWindow::on_importToolButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                   "Select Json File",
                                                   "",
                                                   tr("Json (*.json)"));
    if (!fileName.isEmpty())
    {
        matchStatusModel->importInfo(fileName);
    }
}

void MainWindow::on_englishToolButton_clicked()
{
    QSettings settings;
    settings.setValue("selected_language", "en_EN");
    updateData();
}

void MainWindow::on_spanishToolButton_clicked()
{
    QSettings settings;
    settings.setValue("selected_language", "es_ES");
    updateData();
}

void MainWindow::on_catalaToolButton_clicked()
{
    QSettings settings;
    settings.setValue("selected_language", "ca_ES");
    updateData();
}
