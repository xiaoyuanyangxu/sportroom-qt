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
    QVector<PlayerStat> list;

    QFile f(fileName);
    f.open(QIODevice::ReadOnly);
    QTextStream s1(&f);


    while (!s1.atEnd()){
        QString s=s1.readLine(); // reads line from file
        QStringList fields = s.split(";");

        if (fields.length() == 12)
        {
            PlayerStat stat;
            stat.name = fields[0].trimmed();
            stat.teamName = fields[1].trimmed();

            stat.gamePlayed = fields[3].toInt();
            stat.winGames = fields[4].toInt();
            stat.pointPlayed = fields[6].toInt();
            stat.winPoints = fields[7].toInt();
            stat.matchPlayed = fields[9].toInt();
            stat.winMatch = fields[10].toInt();

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

int PlayerDatamodel::rowCount(const QModelIndex &parent) const
{
    return playerList.size();
}

int PlayerDatamodel::columnCount(const QModelIndex &parent) const
{
    return 11;
}

QVariant PlayerDatamodel::data(const QModelIndex &index, int role) const
{
    PlayerStat p = playerList[index.row()];
    if (role == Qt::DisplayRole)
    {
        switch(index.column())
        {
        case 0:
            return p.name;
        case 1:
            return p.teamName;
        case 2:
            return QString::number(p.gamePlayed);
        case 3:
            return QString("%1(%2%)").arg(p.winGames).arg(float(p.winGames) / float(p.gamePlayed) * 100.0, 5, 'f', 1);
        case 4:
            return QString::number(p.gamePlayed - p.winGames);
        case 5:
            return QString("%1(%2)").arg(p.pointPlayed).arg(p.winPoints - (p.pointPlayed-p.winPoints));
        case 6:
            return QString::number(p.winPoints);
        case 7:
            return QString::number(p.pointPlayed - p.winPoints);
        case 8:
            return QString("%1(%2)").arg(p.matchPlayed).arg(p.winMatch- (p.matchPlayed-p.winMatch));
        case 9:
            return QString::number(p.winMatch);
        case 10:
            return QString::number(p.matchPlayed - p.winMatch);
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
            return tr("Name");
        case 1:
            return tr("Team");
        case 2:
            return tr("GamePlayed");
        case 3:
            return tr("Triumphs");
        case 4:
            return tr("LostGame");
        case 5:
            return tr("PointsPlayed");
        case 6:
            return tr("TriumphPoints");
        case 7:
            return tr("LostPoints");
        case 8:
            return tr("MatchPlayed");
        case 9:
            return tr("TriumphMatches");
        case 10:
            return tr("LostMatches");
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
    if (column == 0) {
        beginResetModel();
        qSort(playerList.begin() , playerList.end() , (order==Qt::AscendingOrder)? nameLessThan: nameBiggerThan);
        endResetModel();
        return;
    }
    if (column == 1) {
        beginResetModel();
        qSort(playerList.begin() , playerList.end() ,(order==Qt::AscendingOrder)? teamLessThan:teamBiggerThan);
        endResetModel();
        return;
    }
}

QString PlayerStat::toString(QString separator)
{
    QStringList fields;

    fields << name;
    fields << teamName;
    fields << QString::number(winGames * 100 / gamePlayed);
    fields << QString::number(gamePlayed);
    fields << QString::number(winGames);
    fields << QString::number(gamePlayed - winGames);
    fields << QString::number(pointPlayed);
    fields << QString::number(winPoints);
    fields << QString::number(pointPlayed - winPoints);
    fields << QString::number(matchPlayed);
    fields << QString::number(winMatch);
    fields << QString::number(matchPlayed - winMatch);

    qDebug() << Q_FUNC_INFO <<  fields.join(separator);

    return fields.join(separator);
}
