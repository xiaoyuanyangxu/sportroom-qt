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
    matchStatusModel->getPoints(currentGame, currentMatch, playerAPoint, playerBPoint);
    matchStatusModel->getPlayerName(currentGame, playerAName, playerBName);
    ui->teamANameLineEdit->setText(matchStatusModel->getTeamAName());
    ui->teamBNameLineEdit->setText(matchStatusModel->getTeamBName());

    drawImage(ui->teamBLogoLabel, matchStatusModel->getTeamBLogoFile());
    drawImage(ui->teamALogoLabel, matchStatusModel->getTeamALogoFile());
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

    QStringList allTeams = {"CTM TRANVIA DE MURCIA",
            "MARPEX BERAUN ERRENTERIA T.M.",
            "IMATEC GABINETES RADIOLOGICOS CARTAGENA",
            "ATLETICO SAN SEBASTIAN",
            "QUADIS CENTRE NATACIO MATARO",
            "C.T.T. OLOT - GABINET PERMAR",
            "SOSMATIC BADALONA",
            "CLUB TENNIS TAULA BORGES",
            "TERMOTUR CALELLA",
            "IRUN LEKA ENEA",
            "CLUB NATACIO SABADELL",
            "FRANCISCO R. POMARES S.L."};

    QCompleter *completer = new QCompleter(allTeams, this);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    ui->teamANameLineEdit->setCompleter(completer);
    ui->teamBNameLineEdit->setCompleter(completer);
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

void MainWindow::drawImage(QLabel *label, QString uri)
{
    if (uri.isEmpty()) return;

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

void MainWindow::on_matchResultTableView_doubleClicked(const QModelIndex &index)
{
    if (index.column() == 1 || index.column() == 3)
    {
        if (index.row() == 6 || (index.row() >= 0 && index.row() <=2))
        {
            bool ok;
            QString aName, bName;

            matchStatusModel->getPlayerName(index.row(), aName, bName);

            QString text = QInputDialog::getText(this, tr("Player Name"),
                                                 tr("Player Name:"), QLineEdit::Normal,
                                                 (index.column()==1)?aName:bName, &ok);
            if (ok && !text.isEmpty()) {
                if (index.column() == 1 )
                {
                    int otherGame[3] = {3,5,4};
                    matchStatusModel->setPlayerAName(index.row(), text);
                    if (index.row() != 6)
                    {
                        matchStatusModel->setPlayerAName(otherGame[index.row()], text);
                    }
                }else{
                    int otherGame[3] = {4,3,5};
                    matchStatusModel->setPlayerBName(index.row(), text);
                    if (index.row() != 6)
                    {
                        matchStatusModel->setPlayerBName(otherGame[index.row()], text);
                    }
                }

            }
        }
    } else if (index.column() >= 4 && index.column()<=8)
    {
        currentGame = index.row();
        currentMatch = index.column() - 4;

        matchStatusModel->setCurrentMatch(currentGame, currentMatch);
        matchStatusModel->setPoints(currentGame, currentMatch, 0,0);
        //updateData();
    }
}

void MainWindow::on_playerAUpPushButton_clicked()
{
    playerAPoint ++;
    matchStatusModel->setPoints(currentGame, currentMatch, playerAPoint, playerBPoint);
}

void MainWindow::on_playerADownPushButton_clicked()
{
    playerAPoint  = std::max(playerAPoint-1, 0);

    matchStatusModel->setPoints(currentGame, currentMatch, playerAPoint, playerBPoint);
}

void MainWindow::on_playerAResetPushButton_clicked()
{
    playerAPoint = 0 ;
    matchStatusModel->setPoints(currentGame, currentMatch, playerAPoint, playerBPoint);
}

void MainWindow::on_playerBUpPushButton_clicked()
{
    playerBPoint ++;
    matchStatusModel->setPoints(currentGame, currentMatch, playerAPoint, playerBPoint);
}

void MainWindow::on_playerBDownPushButton_clicked()
{
    playerBPoint  = std::max(playerBPoint-1, 0);
    matchStatusModel->setPoints(currentGame, currentMatch, playerAPoint, playerBPoint);
}

void MainWindow::on_playerBResetPushButton_clicked()
{
    playerBPoint = 0;
    matchStatusModel->setPoints(currentGame, currentMatch, playerAPoint, playerBPoint);
}



void MainWindow::on_matchResultTableView_clicked(const QModelIndex &index)
{
    if (index.column() >= 4 && index.column()<=8)
    {
        currentGame = index.row();
        currentMatch = index.column() - 4;

        matchStatusModel->getPoints(currentGame, currentMatch, playerAPoint, playerBPoint);
        matchStatusModel->getPlayerName(currentGame, playerAName, playerBName);
        matchStatusModel->setCurrentMatch(currentGame, currentMatch);
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
        //drawImage(ui->teamBLogoLabel, fileName);
    }
}

void MainWindow::on_exchangePushButton_clicked()
{
    matchStatusModel->exchangeTeam();
}

void MainWindow::on_importPushButton_clicked()
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

void MainWindow::on_exportPushButton_clicked()
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

    dialog->show();

    connect(dialog,
            &FullMatchResultDialog::finished,
            [=](int result){
                Q_UNUSED(result);
                dialog->hide();
                dialog->deleteLater();
    });
}
