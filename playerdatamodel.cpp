#include <QFile>
#include <QTextStream>
#include <QDebug>

#include "playerdatamodel.h"


PlayerDatamodel::PlayerDatamodel()
{

    importPlayerList(PLAYER_DATA_FILE_NAME);
}

int PlayerDatamodel::importPlayerList(QString fileName)
{
    qDebug() << Q_FUNC_INFO <<  fileName;

    QVector<PlayerStat> list;

    QFile f(fileName);
    f.open(QIODevice::ReadOnly);
    QTextStream s1(&f);


    while (!s1.atEnd()){
        QString s=s1.readLine(); // reads line from file
        QStringList fields = s.split(";");

        if (fields.length() >= 13)
        {
            PlayerStat stat;

            stat.id = fields[0].toInt();
            stat.name = fields[1].trimmed();
            stat.teamName = fields[2].trimmed();

            stat.matchPlayed = fields[4].toInt();
            stat.winMatch = fields[5].toInt();

            stat.gamePlayed = fields[7].toInt();
            stat.winGames = fields[8].toInt();

            stat.pointPlayed = fields[10].toInt();
            stat.winPoints = fields[11].toInt();

            if (fields.length() > 13)
            {
                stat.imagePath = fields[13].trimmed();
            }

            list.append(stat);
        }
    }
    f.close();

    qDebug() << Q_FUNC_INFO << "imported: " << list.size();
    beginInsertRows(QModelIndex(), 0,list.size());
    list.swap(playerList);
    endInsertRows();

    saveContent();

    emit contentChanged();

    return playerList.size();
}

void PlayerDatamodel::saveContent()
{

    QFile f(PLAYER_DATA_FILE_NAME);

    f.open(QIODevice::WriteOnly);
    QTextStream s1(&f);

    for (int i = 0 ; i < playerList.size() ; i++)
    {
        s1 << playerList[i].toString(";") << endl;
    }
    f.close();
}

void PlayerDatamodel::getPlayerNameList(QStringList &list)
{
    for (int i = 0 ; i < playerList.size() ; i++)
    {
        list << playerList[i].name;
    }
}

void PlayerDatamodel::getTeamNameList(QStringList &list)
{
    QSet<QString> s;
    for (int i = 0 ; i < playerList.size() ; i++)
    {
        s << playerList[i].teamName;
    }

    for (auto i: s)
    {
        list << i;
    }
}

bool PlayerDatamodel::getPlayerStats(QString name, PlayerStat &stats)
{
    int index;
    if ((index = getPlayerIndex(name)) >= 0)
    {
        stats = playerList[index];
        return true;
    }else{
        stats = PlayerStat();
    }
    return false;
}

bool PlayerDatamodel::setPlayerImagePath(QString name, QString path)
{
    int index;
    if ((index = getPlayerIndex(name)) >= 0)
    {
        playerList[index].imagePath = path;

        qDebug() << Q_FUNC_INFO << index << " SET " << path;

        saveContent();

        emit contentChanged();

        return true;
    }
    qDebug() << Q_FUNC_INFO << index << " " << path;
    return false;
}

void PlayerDatamodel::addPlayerIfNotExit(QString name, QString teamName)
{
    int index;
    if ((index = getPlayerIndex(name)) < 0)
    {

        PlayerStat stat;

        stat.id =  getMaxPlayerIndex() + 1;
        stat.name = name;
        stat.teamName = teamName;

        stat.matchPlayed = 0;
        stat.winMatch = 0;

        stat.gamePlayed = 0;
        stat.winGames = 0;

        stat.pointPlayed = 0;
        stat.winPoints = 0;
        stat.imagePath = "";

        qDebug() << Q_FUNC_INFO << "let add: " << playerList.size();
        beginInsertRows(QModelIndex(), playerList.size(), playerList.size()+1);
        playerList.append(stat);
        endInsertRows();
        qDebug() << Q_FUNC_INFO << "added: " << playerList.size();

        saveContent();

        emit contentChanged();
    }
}

int PlayerDatamodel::getPlayerIndex(QString name)
{
    for (int i = 0 ; i < playerList.size() ; i++)
    {
        if (playerList[i].name.toLower() == name.toLower())
        {
            return i;
        }
    }
    return -1;
}

int PlayerDatamodel::getMaxPlayerIndex()
{
    int index = -1;
    for (int i = 0 ; i < playerList.size() ; i++)
    {
        index = std::max(playerList[i].id, index);
    }
    qDebug() << Q_FUNC_INFO << playerList.size() << " maxIndex " << index;
    return index;
}

int PlayerDatamodel::rowCount(const QModelIndex &parent) const
{
    return playerList.size();
}

int PlayerDatamodel::columnCount(const QModelIndex &parent) const
{
    return 12;
}

QVariant PlayerDatamodel::data(const QModelIndex &index, int role) const
{
    PlayerStat p = playerList[index.row()];
    if (role == Qt::DisplayRole)
    {
        switch(index.column())
        {
        case 0:
            return p.id;
        case 1:
            return p.name;
        case 2:
            return p.teamName;
        case 3:
            return QString::number(p.matchPlayed);
        case 4:
            if (p.matchPlayed > 0)
            {
                return QString("%1(%2%)").arg(p.winMatch).arg(float(p.winMatch) / float(p.matchPlayed) * 100.0, 5, 'f', 1);
            }else{
                return "0(0.0%)";
            }
        case 5:
            return QString::number(p.matchPlayed - p.winMatch);
        case 6:
            return QString("%1(%2)").arg(p.pointPlayed).arg(p.winPoints - (p.pointPlayed-p.winPoints));
        case 7:
            return QString::number(p.winPoints);
        case 8:
            return QString::number(p.pointPlayed - p.winPoints);
        case 9:
            return QString("%1(%2)").arg(p.gamePlayed).arg(p.winGames- (p.gamePlayed-p.winGames));
        case 10:
            return QString::number(p.winGames);
        case 11:
            return QString::number(p.gamePlayed - p.winGames);
        }
    }

    return QVariant();

}

QVariant PlayerDatamodel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if ((role == Qt::DisplayRole) && (orientation == Qt::Horizontal))
    {
        switch(section)
        {
        case 0:
            return tr("Id");
        case 1:
            return tr("Name");
        case 2:
            return tr("Team");
        case 3:
            return tr("MatchPlayed");
        case 4:
            return tr("Triumphs");
        case 5:
            return tr("LostGame");
        case 6:
            return tr("PointsPlayed");
        case 7:
            return tr("TriumphPoints");
        case 8:
            return tr("LostPoints");
        case 9:
            return tr("GamePlayed");
        case 10:
            return tr("TriumphGames");
        case 11:
            return tr("LostGames");
        default:
            return QVariant();
        }
    }
    return QVariant();
}

bool PlayerDatamodel::removeRows(int row, int count, const QModelIndex &parent)
{
    qDebug() << Q_FUNC_INFO << row << " " << count << " " << parent;

    if(row < playerList.size())
    {
        beginRemoveRows(QModelIndex(), row, row);
        playerList.remove(row, count);
        endRemoveRows();

        saveContent();

        emit contentChanged();
    }
}

bool nameLessThan(const PlayerStat &p1, const PlayerStat &p2)
{
    return p1.name.toLower() < p2.name.toLower();
}

bool nameBiggerThan(const PlayerStat &p1, const PlayerStat &p2)
{
    return p1.name.toLower() > p2.name.toLower();
}

bool teamLessThan(const PlayerStat &p1, const PlayerStat &p2)
{
    return p1.teamName.toLower() < p2.teamName.toLower();
}

bool teamBiggerThan(const PlayerStat &p1, const PlayerStat &p2)
{
    return p1.teamName.toLower() > p2.teamName.toLower();
}

void PlayerDatamodel::sort(int column, Qt::SortOrder order)
{
    if (column == 1) {
        beginResetModel();
        qSort(playerList.begin() , playerList.end() , (order==Qt::AscendingOrder)? nameLessThan: nameBiggerThan);
        endResetModel();
        return;
    }
    if (column == 2) {
        beginResetModel();
        qSort(playerList.begin() , playerList.end() ,(order==Qt::AscendingOrder)? teamLessThan:teamBiggerThan);
        endResetModel();
        return;
    }
}

PlayerStat::PlayerStat()
{
    id = 0;
    name = "";
    teamName = "";

    matchPlayed = 0;
    winMatch = 0;
    pointPlayed = 0;
    winPoints = 0;
    gamePlayed = 0;
    winGames = 0;

    imagePath = "";
}

QString PlayerStat::toString(QString separator)
{
    QStringList fields;

    fields << QString::number(id);
    fields << name;
    fields << teamName;

    if (matchPlayed > 0)
    {
        fields << QString::number(winMatch * 100 / matchPlayed);
    }else{
        fields << "0";
    }
    fields << QString::number(matchPlayed);
    fields << QString::number(winMatch);
    fields << QString::number(matchPlayed - winMatch);

    fields << QString::number(gamePlayed);
    fields << QString::number(winGames);
    fields << QString::number(gamePlayed - winGames);

    fields << QString::number(pointPlayed);
    fields << QString::number(winPoints);
    fields << QString::number(pointPlayed - winPoints);

    fields << imagePath;

    //qDebug() << Q_FUNC_INFO <<  fields.join(separator);

    return fields.join(separator);
}
