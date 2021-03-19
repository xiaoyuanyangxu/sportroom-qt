#ifndef PLAYERDATAMODEL_H
#define PLAYERDATAMODEL_H
#include <QAbstractTableModel>


#define PLAYER_DATA_FILE_NAME "./playerData.csv"


class PlayerStat
{
public:
    PlayerStat();

public:
    int id;
    QString name;
    QString teamName;

    int matchPlayed;
    int winMatch;

    int pointPlayed;
    int winPoints;

    int gamePlayed;
    int winGames;

    QString imagePath;
public:
    QString toString(QString separator);
};


class PlayerDatamodel : public QAbstractTableModel
{
    Q_OBJECT
public:
    PlayerDatamodel();

    int importPlayerList(QString fileName);

    void saveContent();

    void getPlayerNameList(QStringList &list);

    void getTeamNameList(QStringList &list);

    bool getPlayerStats(QString name, PlayerStat & stats);

    bool setPlayerImagePath(QString name, QString path);

    void addPlayerIfNotExit(QString name, QString teamName);

private:
    int getPlayerIndex(QString name);

    int getMaxPlayerIndex();

signals:
    void contentChanged();

    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;

private:
    QVector<PlayerStat> playerList;


    // QAbstractItemModel interface
public:
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    // QAbstractItemModel interface
public:
    bool removeRows(int row, int count, const QModelIndex &parent);

    // QAbstractItemModel interface
public:
    void sort(int column, Qt::SortOrder order);
};

#endif // PLAYERDATAMODEL_H
