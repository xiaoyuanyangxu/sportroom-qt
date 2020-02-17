#ifndef MATCHSTATUS_H
#define MATCHSTATUS_H

#include <QAbstractTableModel>



class MatchStatus : public QAbstractTableModel
{
    Q_OBJECT
public:
    MatchStatus();

    void readStatus();

    void getPlayerName(int game, QString &playerA, QString &playerB);
    void setPlayerAName(int game, QString &name);
    void setPlayerBName(int game, QString &name);

    void getPoints(int game, int match, int &playerA, int &playerB);
    void setPoints(int game, int match, int playerA, int playerB);

    QString getTeamAName();
    void setTeamAName(const QString &name);
    QString getTeamBName();
    void setTeamBName(const QString &name);

    void setPlayerATimeout(int game, bool timeout);
    void setPlayerBTimeout(int game, bool timeout);
    bool getPlayerATimeout(int game);
    bool getPlayerBTimeout(int game);

    void reset();
    void setCurrentMatch(int game, int match);
    void getCurrentMatch(int & game, int & match);

    void getCurrentResult(int& teamA, int& teamB, int until=7) const;
    void getCurrentGameResult(int game, int& teamA, int& teamB) const;

    QString getTeamALogoFile();
    QString getTeamBLogoFile();

    void setTeamALogoFile(const QString &file);
    void setTeamBLogoFile(const QString &file);

    void exchangeTeam();

    void exportInfo(const QString &fileName);
    void importInfo(const QString &fileName);

    void addImage(QString label, QString path);
    QString getImage(QString label);


    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

private:
    void initialize();
    void saveStatus();

signals:
    void contentChanged();

private:
    QString teamAName;
    QString teamBName;

    QString teamALogoFile;
    QString teamBLogoFile;

    QString playerANameList[7];
    QString playerBNameList[7];

    bool playerATimeout[7];
    bool playerBTimeout[7];

    int currentGame;
    int currentMatch;

    int points[7][5][2];

    QMap<QString, QString> imageList;
};

#endif // MATCHSTATUS_H
