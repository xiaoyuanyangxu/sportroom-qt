#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "matchstatus.h"
#include "playerdatamodel.h"

#include <QLabel>
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_currentMatchResultPushButton_clicked();

    void on_matchResultTableView_doubleClicked(const QModelIndex &index);

    void on_playerAUpPushButton_clicked();

    void on_playerADownPushButton_clicked();

    void on_playerAResetPushButton_clicked();

    void on_playerBUpPushButton_clicked();

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

public slots:
    void contentChanged();
    void playerContentChanged();

private:
    void initializeResultTable();

private:
    Ui::MainWindow *ui;

    int playerAPoint;
    int playerBPoint;

    QString playerAName;
    QString playerBName;

    int currentMatch;
    int currentGame;

    int points[7][5][2];

    MatchStatus * matchStatusModel;

    PlayerDatamodel * playerModel;

};
#endif // MAINWINDOW_H
