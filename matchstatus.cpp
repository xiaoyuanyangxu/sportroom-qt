#include "matchstatus.h"

#include <QBrush>
#include <QFile>
#include <QIODevice>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTextStream>

MatchStatus::MatchStatus()
{
    initialize();
}

void MatchStatus::getPlayerName(int game, QString &playerA, QString &playerB)
{
    if (game < 0 || game >= 7) {
        return;
    }
    playerA = playerANameList[game];
    playerB = playerBNameList[game];
}

void MatchStatus::setPlayerAName(int game, QString &name)
{
    if (game < 0 || game >= 7) {
        return;
    }
    playerANameList[game] = name;
    saveStatus();

    emit dataChanged(index(0,0), index(7,11));
    emit contentChanged();
}

void MatchStatus::setPlayerBName(int game, QString &name)
{
    if (game < 0 || game >= 7) {
        return;
    }
    playerBNameList[game] = name;
    saveStatus();
    emit dataChanged(index(0,0), index(7,11));
    emit contentChanged();
}

void MatchStatus::getPoints(int game, int match, int &playerA, int &playerB)
{
    if (game <0 || game >=7 || match <0 || match >=5)
    {
        return;
    }
    playerA = points[game][match][0];
    playerB = points[game][match][1];

}

void MatchStatus::setPoints(int game, int match, int playerA, int playerB)
{
    if (game <0 || game >=7 || match <0 || match >=5)
    {
        return;
    }
    points[game][match][0] = playerA;
    points[game][match][1] = playerB;
    saveStatus();
    emit dataChanged(index(0,0), index(7,11));
    emit contentChanged();
}

QString MatchStatus::getTeamAName()
{
    return teamAName;
}

void MatchStatus::setTeamAName(const QString &name)
{
    teamAName = name;
    saveStatus();
    emit contentChanged();
}

QString MatchStatus::getTeamBName()
{
    return teamBName;
}

void MatchStatus::setTeamBName(const QString &name)
{
    teamBName = name;
    saveStatus();
    emit contentChanged();
}

void MatchStatus::setCurrentMatch(int game, int match)
{
    currentGame = game;
    currentMatch = match;
    emit dataChanged(index(0,0), index(7,11));
    emit contentChanged();
}

void MatchStatus::getCurrentMatch(int &game, int &match)
{
    game = this->currentGame;
    match = this->currentMatch;
}

void MatchStatus::getCurrentResult(int &teamA, int &teamB, int until) const
{
    teamA = 0;
    teamB = 0;
    for (int i = 0 ; i < until ; i++)
    {
        int teamAGameResult, teamBGameResult;
        getCurrentGameResult(i, teamAGameResult, teamBGameResult);
        if (teamAGameResult >=3)
        {
            teamA ++;
        }else if (teamBGameResult >= 3)
        {
            teamB ++;
        }
    }
}

void MatchStatus::getCurrentGameResult(int game, int &teamA, int &teamB) const
{
    if (game <0 || game >=7)
    {
        return;
    }
    teamA = 0;
    teamB = 0;
    for (int i=0 ; i<5 ; i++)
    {
        if (points[game][i][0] >= 11 || points[game][i][1] >= 11)
        {
            if ( points[game][i][0] >= (points[game][i][1] + 2)) {
                teamA ++;
            }else if(points[game][i][1] >= (points[game][i][0] + 2)){
                teamB ++;
            }
        }
    }
}

QString MatchStatus::getTeamALogoFile()
{
    return teamALogoFile;
}

QString MatchStatus::getTeamBLogoFile()
{
    return teamBLogoFile;
}

void MatchStatus::setTeamALogoFile(const QString &file)
{
    teamALogoFile = file;
    saveStatus();
    emit contentChanged();
}

void MatchStatus::setTeamBLogoFile(const QString &file)
{
    teamBLogoFile = file;
    saveStatus();
    emit contentChanged();
}

void MatchStatus::exchangeTeam()
{
    QString t;
    t = teamBName;
    teamBName = teamAName;
    teamAName = t;

    t = teamBLogoFile;
    teamBLogoFile = teamALogoFile;
    teamALogoFile = t;

    for (int i=0 ; i < 7 ; i++)
    {
        for (int j=0 ; j<5 ; j++)
        {
            int p = points[i][j][1];
            points[i][j][1] = points[i][j][0];
            points[i][j][0] = p;
        }
    }
    for (int i = 0 ; i < 7; i++){
        QString n;
        n = playerANameList[i];
        playerANameList[i] = playerBNameList[i];
        playerBNameList[i] = n;
    }
    saveStatus();
    emit dataChanged(index(0,0), index(7,11));
    emit contentChanged();
}

int MatchStatus::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 7;
}

int MatchStatus::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 11;
}

QVariant MatchStatus::data(const QModelIndex &index, int role) const
{
    QStringList teamALabels = {"A","B","C","A","C","B","D"};
    QStringList teamBLabels = {"Y","X","Z","X","Y","Z","D"};
    int teamA, teamB;
    QString name;

    if (role == Qt::DisplayRole)
    {
        switch(index.column())
        {
        case 0:
            return teamALabels[index.row()];
        case 1:
            name = playerANameList[index.row()];
            /*if (index.row() == 6){
                name = name.split("/").join("\n");
            }*/
            return name;
        case 2:
            return teamBLabels[index.row()];
        case 3:
            name = playerBNameList[index.row()];
            //if (index.row() == 6){
            //    name = name.split("/").join("\n");
            //}
            return name;
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
            if (points[index.row()][index.column()-4][0] == 0 && points[index.row()][index.column()-4][1] == 0)
            {
                if (index.row() != currentGame  || currentMatch != (index.column()-4)) {
                    return "";
                }
            }
            return QString().sprintf("%i-%i", points[index.row()][index.column()-4][0], points[index.row()][index.column()-4][1]);
        case 9:
            getCurrentGameResult(index.row(), teamA, teamB);
            if (teamA < 3 && teamB < 3)
            {
                if (index.row() != currentGame)
                {
                    return "";
                }
            }
            return QString().sprintf("%i-%i", teamA, teamB);

        case 10:
            getCurrentGameResult(index.row(), teamA, teamB);
            if (teamA >= 3 || teamB >= 3)
            {
                getCurrentResult(teamA, teamB, index.row()+1);
                return QString().sprintf("%i-%i", teamA, teamB);
            }else{
                return "";
            }
        }
    }

    if (role == Qt::BackgroundRole)
    {
        if (index.column() >= 4 && index.column() <=8)
        {
            if (index.row() == currentGame && (index.column() - 4) == currentMatch)
            {
               return QBrush(Qt::cyan);
            }else{
                return QBrush(Qt::lightGray);
            }
        }
        if (index.column() == 0 || index.column() ==2 )
        {
            return QBrush(Qt::darkGray);
        }
        if (index.column() == 10 )
        {
            return QBrush(Qt::yellow);
        }
    }

    if (role == Qt::TextAlignmentRole)
    {
        switch(index.column())
        {
        case 0:
        case 2:
            return QVariant((int)(Qt::AlignVCenter|Qt::AlignRight));
        case 1:
        case 3:
            return QVariant((int)(Qt::AlignVCenter|Qt::AlignLeft));
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
            return QVariant((int)(Qt::AlignVCenter|Qt::AlignHCenter));
        }
    }

    return QVariant();
}

QVariant MatchStatus::headerData(int section, Qt::Orientation orientation, int role) const
{
    if ((role == Qt::DisplayRole) && (orientation == Qt::Horizontal))
    {
        switch(section)
        {
        case 0:
            return tr("");
        case 1:
            return teamAName;
        case 2:
            return tr("");
        case 3:
            return teamBName;
        case 4:
            return tr("J1");
        case 5:
            return tr("J2");
        case 6:
            return tr("J3");
        case 7:
            return tr("J4");
        case 8:
            return tr("J5");
        case 9:
            return tr("JOCS");
        case 10:
            return tr("TOTAL");
        default:
            return QVariant();
        }
    }
    return QVariant();
}

void MatchStatus::initialize()
{
    currentGame = 0;
    currentMatch = 0;
    teamAName = "";
    teamBName = "";
    teamALogoFile = "";
    teamBLogoFile = "";

    int* p = &(points[0][0][0]);
    for (int i=0 ; i < 7*5*2 ; i++)
    {
        *p = 0;
        p ++;
    }
    for (int i = 0 ; i < 7; i++){
        playerANameList[i] = "";
        playerBNameList[i] = "";
    }
}

void MatchStatus::reset()
{
    initialize();
    saveStatus();

    emit contentChanged();
}

void MatchStatus::exportInfo(const QString &fileName)
{
    QFile saveFile(fileName);

    if (!saveFile.open(QIODevice::WriteOnly)) {
        qWarning("Couldn't open save file.");
        return;
    }

    QJsonArray allMatches;

    for (int i = 0 ; i < 7; i++){
        QJsonObject matchObject;
        matchObject["playerA"] = playerANameList[i];
        matchObject["playerB"] = playerBNameList[i];

        QJsonArray gameArray;
        for (int g=0 ; g<5 ; g++)
        {
            QJsonObject result;
            result["playerA"] = points[i][g][0];
            result["playerB"] = points[i][g][1];
            gameArray.append(result);
        }
        matchObject["points"] = gameArray;
        allMatches.append(matchObject);
    }

    QJsonObject doc;
    doc["matches"] = allMatches;
    doc["teamA"] = teamAName;
    doc["teamB"] = teamBName;
    doc["teamALogoFile"] = teamALogoFile;
    doc["teamBLogoFile"] = teamBLogoFile;

    QJsonDocument saveDoc(doc);
    saveFile.write(saveDoc.toJson());

    return;
}

void MatchStatus::importInfo(const QString &fileName)
{
    QFile loadFile(fileName);

    if (!loadFile.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open save file.");
        return;
    }

    QByteArray saveData = loadFile.readAll();

    QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));

    QJsonObject obj = loadDoc.object();
    QJsonArray allMatches = obj["matches"].toArray();
    teamAName = obj["teamA"].toString();
    teamBName = obj["teamB"].toString();
    teamALogoFile = obj["teamALogoFile"].toString();
    teamBLogoFile = obj["teamBLogoFile"].toString();

    for (int i = 0 ; i < 7 && i<allMatches.size(); i++){
        QJsonObject matchObject;
        matchObject = allMatches[i].toObject();
        playerANameList[i] = matchObject["playerA"].toString();
        playerBNameList[i] = matchObject["playerB"].toString();

        QJsonArray gameArray = matchObject["points"].toArray();
        for (int g=0 ; g<5 && g< gameArray.size(); g++)
        {
            QJsonObject result = gameArray[g].toObject();
            points[i][g][0] = result["playerA"].toInt();
            points[i][g][1] = result["playerB"].toInt();
        }
    }
}


void MatchStatus::saveStatus()
{
    exportInfo(QStringLiteral("save.json"));
}

void MatchStatus::readStatus()
{
    importInfo(QStringLiteral("save.json"));
}
