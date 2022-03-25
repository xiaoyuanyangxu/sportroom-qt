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
#include "multifuntionaldialog.h"

#include "sportroomutils.h"
#include "playerandteamsdialog.h"
#include "selectplayerdialog.h"
#include "playerstatsdialog.h"
#include "statusmarkdialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      matchStatusModel(0),
      stateModel(0),
      playerModel(0),
      reflectorConnector(0)
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

    bool playerAServe = matchStatusModel->getPlayerAServe(currentMatch);
    bool swapped = matchStatusModel->getSwapSide();
    ui->serveAToolButton->setChecked(!swapped?(playerAServe):(!playerAServe));
    ui->serveBToolButton->setChecked(!swapped?(!playerAServe):(playerAServe));

}

void MainWindow::updateVersionLabel()
{
    QString versionStr = QString("%1-%2-%3")
                                .arg(long(matchStatusModel->getCurrentVersion()%100000),5,10,QChar('0'))
                                .arg(long(playerModel->getCurrentVersion()%100000),5,10,QChar('0'))
                                .arg(long(stateModel->getCurrentVersion()%100000),5,10,QChar('0'));
    ui->versionLabel->setText(versionStr);
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
    CurrentMatchResultDialog *dialog = new CurrentMatchResultDialog(matchStatusModel, stateModel, NULL);

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
    reflectorConnector = new ReflectorConnector(matchStatusModel, stateModel, playerModel, this);

    QObject::connect(reflectorConnector, &ReflectorConnector::stateChanged,
                     this, &MainWindow::reflectorStateChanged);

}

void MainWindow::closeEvent(QCloseEvent *event)
{
    exit(0);
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
    if (!matchStatusModel->getSwapSide())
    {
        playerAPoint  = std::max(playerAPoint-1, 0);
    }else{
        playerBPoint  = std::max(playerBPoint-1, 0);
    }

    matchStatusModel->setPoints(currentMatch, currentGame, playerAPoint, playerBPoint);
}

void MainWindow::on_playerAResetPushButton_clicked()
{
    if (!matchStatusModel->getSwapSide())
    {
        playerAPoint = 0 ;
    }else{
        playerBPoint = 0;
    }
    matchStatusModel->setPoints(currentMatch, currentGame, playerAPoint, playerBPoint);
}

void MainWindow::on_playerBDownPushButton_clicked()
{
    if (!matchStatusModel->getSwapSide())
    {
        playerBPoint  = std::max(playerBPoint-1, 0);
    }else{
        playerAPoint  = std::max(playerAPoint-1, 0);
    }
    matchStatusModel->setPoints(currentMatch, currentGame, playerAPoint, playerBPoint);
}

void MainWindow::on_playerBResetPushButton_clicked()
{
    if (!matchStatusModel->getSwapSide())
    {
        playerBPoint = 0 ;
    }else{
        playerAPoint = 0;
    }
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
    updateVersionLabel();
}

void MainWindow::playerContentChanged()
{
    QStringList allTeams;
    playerModel->getTeamNameList(allTeams);

    QCompleter *completer = new QCompleter(allTeams, this);

    completer->setCaseSensitivity(Qt::CaseInsensitive);

    ui->teamANameLineEdit->setCompleter(completer);
    ui->teamBNameLineEdit->setCompleter(completer);
    updateVersionLabel();
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

        if (status == 1) {
            ui->status1ToolButton->setIcon(active);
        }else{
            ui->status1ToolButton->setIcon(noActive);
        }
        if (status == 2) {
            ui->status2ToolButton->setIcon(active);
        }else{
            ui->status2ToolButton->setIcon(noActive);
        }
        if (status == 4) {
            ui->status3ToolButton->setIcon(active);
        }else{
            ui->status3ToolButton->setIcon(noActive);
        }
        if (status == 8) {
            ui->status4ToolButton->setIcon(active);
        }else{
            ui->status4ToolButton->setIcon(noActive);
        }

        ui->localUpdateDelayHorizontalSlider->setValue(stateModel->getLocalUpdateDelay());
        ui->globalUpdateDelayHorizontalSlider->setValue(stateModel->getGlobalUpdateDelay());
        ui->localDelayLabel->setText(QString("%1").arg(stateModel->getLocalUpdateDelay()));
        ui->globalDelayLabel->setText(QString("%1").arg(stateModel->getGlobalUpdateDelay()));
        ui->matchSyncCheckBox->setChecked(stateModel->getMatchSyncPushSelected());

        if (stateModel->getLocalUpdateDelaySelected()) {
            ui->localDelayCheckBox->setChecked(true);
            ui->globalDelayCheckBox->setChecked(false);
        }else{
            ui->localDelayCheckBox->setChecked(false);
            ui->globalDelayCheckBox->setChecked(true);
        }

        int multifuntionalScreenState = stateModel->getMultifunctionaScreenState();

        //ui->screen1ToolButton->setStyleSheet(QString("QToolButton:selected { background-color: rgb(255, 255, 0); }"));
        ui->layer5ToolButton->setChecked(multifuntionalScreenState==5);
        ui->layer4ToolButton->setChecked(multifuntionalScreenState==4);
        ui->layer3ToolButton->setChecked(multifuntionalScreenState==3);
        ui->layer2ToolButton->setChecked(multifuntionalScreenState==2);
        ui->layer1ToolButton->setChecked(multifuntionalScreenState==1);
        ui->hideAllToolButton->setChecked(multifuntionalScreenState==0);
        ui->teamResultToolButton->setChecked(stateModel->getElementState(0x01));
        ui->matchResultToolButton->setChecked(stateModel->getElementState(0x02));
        ui->statusMarkToolButton->setChecked(stateModel->getElementState(0x04));
        ui->showServeToolButton->setChecked(stateModel->getElementState(0x08));

        ui->lazyModeToolButton->setChecked(stateModel->getElementState(0x10));

        updateVersionLabel();
    }
}

void MainWindow::reflectorStateChanged()
{
    qDebug() << Q_FUNC_INFO;
    if(reflectorConnector) {
        bool connected, reconnecting, closed;
        QString id;
        reflectorConnector->getState(connected, reconnecting, closed, id);
        ui->pushPushButton->setEnabled(connected);
        ui->pullPushButton->setEnabled(connected);

        qDebug() << Q_FUNC_INFO<< connected<< reconnecting << closed << id;

        if (!closed) {
            QIcon closeIcon(":/images/delete.png");
            ui->syncPushButton->setIcon(closeIcon);
            ui->syncPushButton->setText(QString("%1: %2").arg(reconnecting?tr("Reconnect:"):tr("Close:")).arg(id));

        }else{
            QIcon syncIcon(":/images/sync_cloud.png");

            ui->syncPushButton->setIcon(syncIcon);
            ui->syncPushButton->setText(tr("Sync"));
        }
    }

}

void MainWindow::on_teamResultPushButton_clicked()
{
    TeamResultDialog *dialog = new TeamResultDialog(matchStatusModel, stateModel, NULL);

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
    QSettings settings;
    QString fileName = QFileDialog::getOpenFileName(this,
                                                   "Select Image File",
                                                   settings.value("asset_folder","").toString(),
                                                   tr("Images (*.png *.png)"));

    if (!fileName.isEmpty())
    {
        matchStatusModel->setTeamALogoFile(fileName);
        //drawImage(ui->teamALogoLabel, fileName);
    }
}

void MainWindow::on_teamBLogoPushButton_clicked()
{
    QSettings settings;
    QString fileName = QFileDialog::getOpenFileName(this,
                                                   "Select Image File",
                                                   settings.value("asset_folder","").toString(),
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
    if (!matchStatusModel->getSwapSide())
    {
        bool timeout = matchStatusModel->getPlayerATimeout(currentMatch);
        matchStatusModel->setPlayerATimeout(currentMatch, !timeout);
    }else{
        bool timeout = matchStatusModel->getPlayerBTimeout(currentMatch);
        matchStatusModel->setPlayerBTimeout(currentMatch, !timeout);

    }
}

void MainWindow::on_playerBTimeoutPushButton_clicked()
{
    if (!matchStatusModel->getSwapSide())
    {
        bool timeout = matchStatusModel->getPlayerBTimeout(currentMatch);
        matchStatusModel->setPlayerBTimeout(currentMatch, !timeout);
    }else{
        bool timeout = matchStatusModel->getPlayerATimeout(currentMatch);
        matchStatusModel->setPlayerATimeout(currentMatch, !timeout);
    }
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
    StatusMarkDialog *dialog = new StatusMarkDialog(matchStatusModel, stateModel, NULL);

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
    if (playerAPoint > 10 || playerBPoint > 10){
        if (std::abs(playerAPoint - playerBPoint) >= 2){
            return ;
        }
    }
    if (!matchStatusModel->getSwapSide())
    {
        playerBPoint ++;
    }else{
        playerAPoint ++;
    }

    matchStatusModel->setPoints(currentMatch, currentGame, playerAPoint, playerBPoint);
    stateModel->setCurrentStatus(!matchStatusModel->getSwapSide()?5:6);
}

void MainWindow::on_playerAUpToolButton_clicked()
{
    if (playerAPoint > 10 || playerBPoint > 10){
        if (std::abs(playerAPoint - playerBPoint) >= 2){
            return ;
        }
    }
    if (!matchStatusModel->getSwapSide())
    {
        playerAPoint ++;

    }else{
        playerBPoint ++;
    }
    matchStatusModel->setPoints(currentMatch, currentGame, playerAPoint, playerBPoint);
    stateModel->setCurrentStatus(!matchStatusModel->getSwapSide()?6:5);

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
        bool connected, reconnecting, closed;
        QString id;

        reflectorConnector->getState(connected, reconnecting, closed, id);
        if (closed) {
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

void MainWindow::on_multifunctionalPushButton_clicked()
{
    MultifuntionalDialog *dialog = new MultifuntionalDialog(matchStatusModel,
                                                            stateModel,
                                                            playerModel, NULL);

    SportRoomUtils::recoverSize(dialog, "multifunctional_dialog");
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


void MainWindow::on_hideAllToolButton_clicked()
{
    stateModel->setMultifunctionaScreenState(0);
}

void MainWindow::on_layer1ToolButton_clicked()
{
    stateModel->setMultifunctionaScreenState(1);
}

void MainWindow::on_layer2ToolButton_clicked()
{
   stateModel->setMultifunctionaScreenState(2);
}

void MainWindow::on_layer3ToolButton_clicked()
{
    stateModel->setMultifunctionaScreenState(3);
}

void MainWindow::on_layer4ToolButton_clicked()
{
     stateModel->setMultifunctionaScreenState(4);
}

void MainWindow::on_layer5ToolButton_clicked()
{
    stateModel->setMultifunctionaScreenState(5);
}

void MainWindow::on_teamResultToolButton_clicked()
{
    bool current = stateModel->getElementState(0x01);
    stateModel->setElementState(0x01, !current);
}

void MainWindow::on_matchResultToolButton_clicked()
{
    bool current = stateModel->getElementState(0x02);
    stateModel->setElementState(0x02, !current);
}


void MainWindow::on_statusMarkToolButton_clicked()
{
    bool current = stateModel->getElementState(0x04);
    stateModel->setElementState(0x04, !current);
}

void MainWindow::on_swapToolButton_clicked()
{
    matchStatusModel->setSwapSide(!matchStatusModel->getSwapSide());
}

void MainWindow::on_serveAToolButton_clicked()
{
    int match, game;
    matchStatusModel->getCurrentMatch(match, game);
    matchStatusModel->setPlayerAServe(match, !matchStatusModel->getSwapSide());
}

void MainWindow::on_serveBToolButton_clicked()
{
    int match, game;
    matchStatusModel->getCurrentMatch(match, game);
    matchStatusModel->setPlayerAServe(match, matchStatusModel->getSwapSide());
}

void MainWindow::on_teamALogoDeletePushButton_clicked()
{
    matchStatusModel->setTeamALogoFile("");
}

void MainWindow::on_teamBLogoDeletePushButton_clicked()
{
    matchStatusModel->setTeamBLogoFile("");
}

void MainWindow::on_showServeToolButton_clicked()
{
    bool current = stateModel->getElementState(0x08);
    stateModel->setElementState(0x08, !current);
}

void MainWindow::on_lazyModeToolButton_clicked()
{
    bool current = stateModel->getElementState(0x10);
    stateModel->setElementState(0x10, !current);
}

void MainWindow::on_globalUpdateDelayHorizontalSlider_valueChanged(int value)
{
    qDebug() << Q_FUNC_INFO << value;
    if (stateModel->getGlobalUpdateDelay() != value)
    {
        stateModel->setGlobalUpdateDelay(value);
    }
    updateData();
}

void MainWindow::on_localUpdateDelayHorizontalSlider_valueChanged(int value)
{
    qDebug() << Q_FUNC_INFO << value;
    if (stateModel->getLocalUpdateDelay() != value)
    {
        stateModel->setLocalUpdateDelay(value);
    }
    updateData();
}

void MainWindow::on_globalDelayCheckBox_clicked(bool checked)
{
    stateModel->setLocalUpdateDelaySelected(!checked);
    updateData();
}

void MainWindow::on_localDelayCheckBox_clicked(bool checked)
{
    stateModel->setLocalUpdateDelaySelected(checked);
    updateData();
}

void MainWindow::on_matchSyncCheckBox_clicked(bool checked)
{
    stateModel->setMatchSyncPushSelected(checked);
    updateData();
}


void MainWindow::on_assertDirPushButton_clicked()
{
    QSettings settings;

    QString dir = QFileDialog::getExistingDirectory(this,
                                                    "",
                                                    settings.value("asset_folder","").toString(),
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);
    if (!dir.isEmpty())
    {
        settings.setValue("asset_folder", dir);
    }
}
