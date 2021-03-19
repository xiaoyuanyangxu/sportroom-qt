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
#include "statusmarkdialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), matchStatusModel(0), stateModel(0), playerModel(0), reflectorConnector(0)
{
    ui->setupUi(this);

    initState();
    updateData();
    initializeResultTable();
    initializeStateTable();
    initializeRefrector();
}

MainWindow::~MainWindow()
{
    delete ui;

    delete matchStatusModel;
    delete stateModel;
    delete playerModel;
    delete reflectorConnector;
}

void MainWindow::updateData()
{
    qDebug() << Q_FUNC_INFO;

    matchStatusModel->getCurrentMatch(currentMatch, currentGame);

    matchStatusModel->getPoints(currentMatch, currentGame, playerAPoint, playerBPoint);
    matchStatusModel->getPlayerName(currentMatch, playerAName, playerBName);

    QString teamA, teamB;
    teamA =  matchStatusModel->getTeamAName();
    teamB =  matchStatusModel->getTeamBName();

    ui->teamANameLineEdit->setText(teamA);
    ui->teamBNameLineEdit->setText(teamB);

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

    qDebug() << Q_FUNC_INFO;
    playerAName = "";
    playerBName = "";
    playerAPoint = 0;
    playerBPoint = 0;

    currentGame  = 0;
    currentMatch = 0;

    matchStatusModel = new MatchStatus();
    qDebug() << Q_FUNC_INFO<< "create state model";
    stateModel = new StateDatamodel();

    qDebug() << Q_FUNC_INFO<< "create player model";
    playerModel = new PlayerDatamodel();

    QObject::connect(matchStatusModel, &MatchStatus::contentChanged,
                     this, &MainWindow::contentChanged);

    ui->scoreboardWidget->setStatusModel(matchStatusModel);

    QObject::connect(playerModel, &PlayerDatamodel::contentChanged,
                     this, &MainWindow::playerContentChanged);
    QObject::connect(stateModel, &StateDatamodel::contentChanged,
                     this, &MainWindow::stateContentChanged);

    matchStatusModel->readStatus();
    playerContentChanged();
    stateContentChanged();

}


void MainWindow::on_currentMatchResultPushButton_clicked()
{
    CurrentMatchResultDialog *dialog = new CurrentMatchResultDialog(matchStatusModel, NULL);

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

void MainWindow::initializeStateTable()
{
    ui->statusTableView->setModel(this->stateModel);

    ui->statusTableView->verticalHeader()->hide();
    ui->statusTableView->setShowGrid(true);

    ui->statusTableView->setSelectionMode(QAbstractItemView::NoSelection);
    ui->statusTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->statusTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);


    ui->statusTableView->setColumnWidth(0, 140);
    ui->statusTableView->setColumnWidth(1, 20);
    ui->statusTableView->hideColumn(1);
    ui->statusTableView->setColumnWidth(2, 60);
    ui->statusTableView->setColumnWidth(3, 50);
    ui->statusTableView->setColumnWidth(4, 60);
    ui->statusTableView->hideColumn(4);
}

void MainWindow::initializeRefrector()
{
    reflectorConnector = new ReflectorConnector(matchStatusModel, stateModel, this);

    QObject::connect(reflectorConnector, &ReflectorConnector::stateChanged,
                     this, &MainWindow::reflectorStateChanged);

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

                            QString teamName;

                            if (index.column() == 1){
                                matchStatusModel->setPlayerAName(index.row(), text);
                                teamName = matchStatusModel->getTeamAName();
                            }else{
                                matchStatusModel->setPlayerBName(index.row(), text);
                                teamName = matchStatusModel->getTeamBName();
                            }

                            playerModel->addPlayerIfNotExit(text, teamName);

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

void MainWindow::stateContentChanged()
{
    qDebug() << Q_FUNC_INFO << "update state buttons" << stateModel;
    if (stateModel)
    {
        int status = stateModel->getCurrentStatus();

        qDebug() << Q_FUNC_INFO << "Status: "<< status;

        QIcon active(":/images/table_tennis_icon.png");
        QIcon noActive(":/images/pingpong.png");

        if (status & 0x01) {
            ui->status1ToolButton->setIcon(active);
        }else{
            ui->status1ToolButton->setIcon(noActive);
        }
        if (status & 0x02) {
            ui->status2ToolButton->setIcon(active);
        }else{
            ui->status2ToolButton->setIcon(noActive);
        }
        if (status & 0x04) {
            ui->status3ToolButton->setIcon(active);
        }else{
            ui->status3ToolButton->setIcon(noActive);
        }
        if (status & 0x08) {
            ui->status4ToolButton->setIcon(active);
        }else{
            ui->status4ToolButton->setIcon(noActive);
        }
    }
}

void MainWindow::reflectorStateChanged()
{
    qDebug() << Q_FUNC_INFO;
    if(reflectorConnector) {
        bool connected;
        QString id;
        reflectorConnector->getState(connected, id);
        ui->pushPushButton->setEnabled(connected);
        ui->pullPushButton->setEnabled(connected);

        if (connected) {
            QIcon closeIcon(":/images/delete.png");
            ui->syncPushButton->setIcon(closeIcon);
            ui->syncPushButton->setText(QString("%1: %2").arg(tr("Close:")).arg(id));

        }else{
            QIcon syncIcon(":/images/sync_cloud.png");

            ui->syncPushButton->setIcon(syncIcon);
            ui->syncPushButton->setText(tr("Sync"));
        }
    }

}

void MainWindow::on_teamResultPushButton_clicked()
{
    TeamResultDialog *dialog = new TeamResultDialog(matchStatusModel, NULL);

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
    bool timeout = matchStatusModel->getPlayerATimeout(currentMatch);
    matchStatusModel->setPlayerATimeout(currentMatch, !timeout);
}

void MainWindow::on_playerBTimeoutPushButton_clicked()
{
    bool timeout = matchStatusModel->getPlayerBTimeout(currentMatch);
    matchStatusModel->setPlayerBTimeout(currentMatch, !timeout);
}

void MainWindow::on_fullResultPushButton_clicked()
{
    FullMatchResultDialog *dialog = new FullMatchResultDialog(matchStatusModel, NULL);

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
    PlayerStatsDialog *dialog = new PlayerStatsDialog(matchStatusModel, playerModel, NULL);

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

void MainWindow::on_statusMarkPushButton_clicked()
{
    StatusMarkDialog *dialog = new StatusMarkDialog(stateModel, NULL);

    SportRoomUtils::recoverSize(dialog, "status_mark");
    dialog->setWindowFlags(Qt::Window);
    dialog->show();

    connect(dialog,
            &StatusMarkDialog::finished,
            [=](int result){
                Q_UNUSED(result);
                dialog->hide();
                dialog->deleteLater();
    });
}

void MainWindow::on_status1ToolButton_clicked()
{
    qDebug() << Q_FUNC_INFO;
    stateModel->setCurrentStatus(1);
}

void MainWindow::on_status2ToolButton_clicked()
{
    qDebug() << Q_FUNC_INFO;
    stateModel->setCurrentStatus(2);
}

void MainWindow::on_status3ToolButton_clicked()
{
    qDebug() << Q_FUNC_INFO;
    stateModel->setCurrentStatus(4);
}

void MainWindow::on_status4ToolButton_clicked()
{
    qDebug() << Q_FUNC_INFO;
    stateModel->setCurrentStatus(8);
}

void MainWindow::on_playerBUpToolButton_clicked()
{
    playerBPoint ++;
    matchStatusModel->setPoints(currentMatch, currentGame, playerAPoint, playerBPoint);
}

void MainWindow::on_playerAUpToolButton_clicked()
{
    playerAPoint ++;
    matchStatusModel->setPoints(currentMatch, currentGame, playerAPoint, playerBPoint);

}

void MainWindow::on_resetToolButton_clicked()
{
    stateModel->reset();
}

void MainWindow::on_pullPushButton_clicked()
{
    if(reflectorConnector) {
        reflectorConnector->pull();
    }
}

void MainWindow::on_pushPushButton_clicked()
{
    if(reflectorConnector) {
        reflectorConnector->push();
    }
}

void MainWindow::on_syncPushButton_clicked()
{
    if(reflectorConnector) {
        bool connected;
        QString id;

        reflectorConnector->getState(connected, id);
        if (!connected) {
            bool ok;

            id = QInputDialog::getText(this, tr("Reflector ID"),
                                  tr("ID:"), QLineEdit::Normal,
                                  id, &ok);
            if (ok) {
                reflectorConnector->connect2Reflector(QUrl(QString(REFRECTOR_BASE_URL) + "reflect/" + id));
            }
         }else{
            reflectorConnector->close();
        }
    }
}
