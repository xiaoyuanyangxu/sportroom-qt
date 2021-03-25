#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "matchstatus.h"
#include "playerdatamodel.h"
#include "reflectorconnector.h"

#include <QLabel>
#include <QMainWindow>
#include "statedatamodel.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

#define REFRECTOR_BASE_URL "ws://api.ootoyasushi.com:3000/"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_currentMatchResultPushButton_clicked();

    void on_matchResultTableView_doubleClicked(const QModelIndex &index);

    void on_playerADownPushButton_clicked();

    void on_playerAResetPushButton_clicked();

    void on_playerBDownPushButton_clicked();

    void on_playerBResetPushButton_clicked();

    void updateData();

    void initState();

    void on_matchResultTableView_clicked(const QModelIndex &index);

    void on_teamANameLineEdit_editingFinished();

    void on_teamBNameLineEdit_editingFinished();

    void on_newGamePushButton_clicked();

    void on_teamResultPushButton_clicked();

    void on_teamALogoPushButton_clicked();

    void on_teamBLogoPushButton_clicked();

    void on_exchangePushButton_clicked();

    void on_playerATimeoutPushButton_clicked();

    void on_playerBTimeoutPushButton_clicked();

    void on_fullResultPushButton_clicked();

    void on_playerTeamPushButton_clicked();

    void on_playerStatsPushButton_clicked();

    void on_exportToolButton_clicked();

    void on_importToolButton_clicked();

    void on_englishToolButton_clicked();

    void on_spanishToolButton_clicked();

    void on_catalaToolButton_clicked();

    void on_statusMarkPushButton_clicked();

    void on_status1ToolButton_clicked();

    void on_status2ToolButton_clicked();

    void on_status3ToolButton_clicked();

    void on_status4ToolButton_clicked();

    void on_playerBUpToolButton_clicked();

    void on_playerAUpToolButton_clicked();

    //void on_sharePushButton_clicked();

    void on_syncPushButton_clicked();

    void on_resetToolButton_clicked();

    void on_pullPushButton_clicked();

    void on_pushPushButton_clicked();

    void on_multifunctionalPushButton_clicked();

    void on_hideAllToolButton_clicked();

    void on_layer1ToolButton_clicked();

    void on_layer2ToolButton_clicked();

    void on_layer3ToolButton_clicked();

    void on_layer4ToolButton_clicked();

    void on_layer5ToolButton_clicked();

    void on_teamResultToolButton_clicked();

    void on_matchResultToolButton_clicked();

    void on_statusMarkToolButton_clicked();

public slots:
    void contentChanged();
    void playerContentChanged();
    void stateContentChanged();
    void reflectorStateChanged();

private:
    void initializeResultTable();
    void initializeStateTable();
    void initializeRefrector();

private:
    Ui::MainWindow *ui;

    int playerAPoint;
    int playerBPoint;

    QString playerAName;
    QString playerBName;

    int currentMatch;
    int currentGame;

    int points[7][5][2];

    MatchStatus *           matchStatusModel;
    StateDatamodel *        stateModel;
    PlayerDatamodel *       playerModel;
    ReflectorConnector *    reflectorConnector;

};
#endif // MAINWINDOW_H
