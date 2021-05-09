#ifndef MATCHSTATUS_H
#define MATCHSTATUS_H

#include <QAbstractTableModel>
#include <QDateTime>
#include <QItemDelegate>
#include <QStyledItemDelegate>




class MatchStatus : public QAbstractTableModel
{
    Q_OBJECT
public:
    MatchStatus();

    void readStatus();

    void getPlayerName(int match, QString &playerA, QString &playerB);
    void setPlayerAName(int match, QString &name);
    void setPlayerBName(int match, QString &name);

    void getPoints(int match, int game, int &playerA, int &playerB);
    void setPoints(int match, int game, int playerA, int playerB);

    QString getTeamAName();
    void setTeamAName(const QString &name);
    QString getTeamBName();
    void setTeamBName(const QString &name);

    void setPlayerATimeout(int match, bool timeout);
    void setPlayerBTimeout(int match, bool timeout);
    bool getPlayerATimeout(int match);
    bool getPlayerBTimeout(int match);

    void setPlayerAServe(int match, bool serveA);
    bool getPlayerAServe(int match);

    void reset();
    void setCurrentMatch(int match, int game);
    void getCurrentMatch(int & match, int & game);

    void getCurrentResult(int& teamA, int& teamB, int until=7) const;
    void getCurrentGameResult(int game, int& teamA, int& teamB) const;

    QString getTeamALogoFile();
    QString getTeamBLogoFile();

    void setTeamALogoFile(const QString &file);
    void setTeamBLogoFile(const QString &file);

    void setSwapSide(bool swapped);
    bool getSwapSide();

    void exchangeTeam();

    void exportInfo(const QString &fileName);
    void importInfo(const QString &fileName);
    QByteArray exportInfoAsJson();
    bool importInfoFromJson(const QByteArray &json);

    void addImage(QString label, QString path);
    QString getImage(QString label);

    int getCurrentVersion(){return version;};

    void setMultifunctionaScreenState(int state);
    int getMultifunctionaScreenState();

    void setElementState(int elementMask, bool enabled);
    bool getElementState(int elementMask);

    void setUpdateDelay(int d);
    int getUpdateDelay() {return updateDelay;};

    void setMultifunctionaScreenLayer3Image(QString path);
    void setMultifunctionaScreenLayer4Image(QString path);
    void setMultifunctionaScreenLayer5Image(QString path);
    QString getMultifunctionaScreenLayer3Image();
    QString getMultifunctionaScreenLayer4Image();

    QString getMultifunctionaScreenLayer5Image();


    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

private:
    void initialize();
    void saveStatus();
    void emitContentChanges();

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
    bool playerAServe[7];

    int currentGame;
    int currentMatch;

    int points[7][5][2];

    int version;

    bool swapped;

    int elementState;
    int updateDelay;

    int multifunctionalScreenState;
    QString layer3Image;
    QString layer4Image;
    QString layer5Image;

    QMap<QString, QString> imageList;
};

#endif // MATCHSTATUS_H
