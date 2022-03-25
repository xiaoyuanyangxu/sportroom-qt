#include "matchstatus.h"

#include <QBrush>
#include <QFile>
#include <QFileInfo>
#include <QIODevice>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QModelIndex>
#include <QPainter>
#include <QSettings>
#include <QTextStream>
#include <QtDebug>
#include <Utils.h>

MatchStatus::MatchStatus()
{
    version = 0;
    initialize();
}

void MatchStatus::getPlayerName(int match, QString &playerA, QString &playerB)
{
    if (match < 0 || match >= 7) {
        return;
    }
    playerA = playerANameList[match];
    playerB = playerBNameList[match];
}

void MatchStatus::setPlayerAName(int match, QString &name)
{
    if (match < 0 || match >= 7) {
        return;
    }
    playerANameList[match] = name;
    saveStatus();

    emit dataChanged(index(0,0), index(7,11));
    emitContentChanges();
}

void MatchStatus::setPlayerBName(int match, QString &name)
{
    if (match < 0 || match >= 7) {
        return;
    }
    playerBNameList[match] = name;
    saveStatus();
    emit dataChanged(index(0,0), index(7,11));
    emitContentChanges();
}

void MatchStatus::getPoints(int match, int game, int &playerA, int &playerB)
{
    if (match <0 || match >=7 || game <0 || game >=5)
    {
        return;
    }
    playerA = points[match][game][0];
    playerB = points[match][game][1];

}

void MatchStatus::setPoints(int match, int game, int playerA, int playerB)
{
    if (match <0 || match >=7 || game <0 || game >=5)
    {
        return;
    }
    points[match][game][0] = playerA;
    points[match][game][1] = playerB;
    saveStatus();
    emit dataChanged(index(0,0), index(7,11));
    emitContentChanges();
}

QString MatchStatus::getTeamAName()
{
    return teamAName;
}

void MatchStatus::setTeamAName(const QString &name)
{
    teamAName = name;
    saveStatus();
    emitContentChanges();
}

QString MatchStatus::getTeamBName()
{
    return teamBName;
}

void MatchStatus::setTeamBName(const QString &name)
{
    teamBName = name;
    saveStatus();
    emitContentChanges();
}

void MatchStatus::setPlayerATimeout(int match, bool timeout)
{
    playerATimeout[match] = timeout;
    saveStatus();
    emitContentChanges();
}

void MatchStatus::setPlayerBTimeout(int match, bool timeout)
{
    playerBTimeout[match] = timeout;
    saveStatus();
    emitContentChanges();
}

bool MatchStatus::getPlayerATimeout(int match)
{
    return playerATimeout[match];
}

bool MatchStatus::getPlayerBTimeout(int match)
{
    return playerBTimeout[match];
}

void MatchStatus::setPlayerAServe(int match, bool serveA)
{
    playerAServe[match] = serveA;
    saveStatus();
    emitContentChanges();
}

bool MatchStatus::getPlayerAServe(int match)
{
    return playerAServe[match];
}

void MatchStatus::setCurrentMatch(int match, int game)
{
    currentGame = game;
    currentMatch = match;
    emit dataChanged(index(0,0), index(7,11));
    emitContentChanges();
}

void MatchStatus::getCurrentMatch(int &match, int &game)
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

int MatchStatus::getCurrentGameResult(int match, int &playerA, int &playerB) const
{
    int maxMatch = -1;

    if (match <0 || match >=7)
    {
        return maxMatch;
    }

    playerA = 0;
    playerB = 0;

    for (int i=0 ; i<5 ; i++)
    {
        if ((points[match][i][0] > 0 || points[match][i][1] > 0) && i > maxMatch) {
            maxMatch = i;
        }

        if (points[match][i][0] >= 11 || points[match][i][1] >= 11)
        {
            if ( points[match][i][0] >= (points[match][i][1] + 2)) {
                playerA ++;
            }else if(points[match][i][1] >= (points[match][i][0] + 2)){
                playerB ++;
            }
        }
    }
    return maxMatch;
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
    emitContentChanges();
}

void MatchStatus::setTeamBLogoFile(const QString &file)
{
    teamBLogoFile = file;
    saveStatus();
    emitContentChanges();
}

void MatchStatus::setSwapSide(bool swapped)
{
    this->swapped = swapped;

    saveStatus();
    emitContentChanges();
}

bool MatchStatus::getSwapSide()
{
    return swapped;
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

        bool timeout;
        timeout = playerATimeout[i];
        playerATimeout[i] = playerBTimeout[i];
        playerBTimeout[i] = timeout;
    }
    saveStatus();
    emit dataChanged(index(0,0), index(7,11));
    emitContentChanges();
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
                if (index.row() != currentMatch  || currentGame != (index.column()-4)) {
                    return "";
                }
            }
            return QString().sprintf("%i-%i", points[index.row()][index.column()-4][0], points[index.row()][index.column()-4][1]);
        case 9:
            getCurrentGameResult(index.row(), teamA, teamB);
            if (teamA < 3 && teamB < 3)
            {
                if (index.row() != currentMatch)
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
            if (index.row() == currentMatch && (index.column() - 4) == currentGame)
            {
               return QBrush(QColor(232, 103, 180));
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
            return QBrush(QColor(232, 103, 180));
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
            return tr("G1");
        case 5:
            return tr("G2");
        case 6:
            return tr("G3");
        case 7:
            return tr("G4");
        case 8:
            return tr("G5");
        case 9:
            return tr("GAMES");
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
    swapped = true;
    //teamAName = "";
    //teamBName = "";
    //teamALogoFile = "";
    //teamBLogoFile = "";

    int* p = &(points[0][0][0]);
    for (int i=0 ; i < 7*5*2 ; i++)
    {
        *p = 0;
        p ++;
    }
    for (int i = 0 ; i < 7; i++){
        playerANameList[i] = "";
        playerBNameList[i] = "";
        playerATimeout[i] = false;
        playerBTimeout[i] = false;
    }
}

void MatchStatus::reset()
{
    initialize();
    saveStatus();

    emit headerDataChanged(Qt::Horizontal, 0, 11);
    emit dataChanged(index(0,0), index(7,11));
    emitContentChanges();
}

void MatchStatus::exportInfo(const QString &fileName)
{
    QFile saveFile(fileName);

    if (!saveFile.open(QIODevice::WriteOnly)) {
        qWarning("Couldn't open save file.");
        return;
    }

    saveFile.write(exportInfoAsJson());
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

    importInfoFromJson(saveData, true);
}

QByteArray MatchStatus::exportInfoAsJson()
{
    QJsonArray allMatches;

    for (int i = 0 ; i < 7; i++){
        QJsonObject matchObject;
        matchObject["playerA"] = playerANameList[i];
        matchObject["playerB"] = playerBNameList[i];
        matchObject["playerATimeout"] = playerATimeout[i];
        matchObject["playerBTimeout"] = playerBTimeout[i];
        matchObject["playerAServe"] = playerAServe[i];

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
    QSettings settings;

    doc["matches"] = allMatches;
    doc["asset_folder"] = settings.value("asset_folder", "").toString();
    doc["teamA"] = teamAName;
    doc["teamB"] = teamBName;
    doc["teamALogoFile"] = teamALogoFile;
    doc["teamBLogoFile"] = teamBLogoFile;
    doc["currentGame"] = currentGame;
    doc["currentMatch"] = currentMatch;

    doc["swapped"] = swapped;

    QJsonArray allImages;

    qDebug() << Q_FUNC_INFO << settings.value("asset_folder", "").toString();

    for (QMap<QString, QString>::iterator it = imageList.begin();
         it != imageList.end() ;
         ++it)
    {
        QJsonObject image;
        image["label"] = it.key();
        image["path"] = it.value();
        allImages.append(image);
    }

    doc["images"] = allImages;

    QJsonDocument saveDoc(doc);

    return saveDoc.toJson();
}

bool MatchStatus::importInfoFromJson(const QByteArray &json, const bool local)
{
    qDebug() << Q_FUNC_INFO << QString(json);
    QSettings settings;

    QString localAssetDir = settings.value("asset_folder", "").toString();
    QString remoteAssetDir;

    QJsonDocument loadDoc(QJsonDocument::fromJson(json));

    QJsonObject obj = loadDoc.object();
    QJsonArray allMatches = obj["matches"].toArray();
    teamAName = obj["teamA"].toString();
    teamBName = obj["teamB"].toString();

    currentGame = obj["currentGame"].toInt();
    currentMatch = obj["currentMatch"].toInt();

    remoteAssetDir = obj["asset_folder"].toString();

    qDebug() << Q_FUNC_INFO << "A: " << teamAName << " B:" << teamBName;

    if (obj.contains("teamALogoFile")) {
        teamALogoFile = obj["teamALogoFile"].toString();
    }
    if (obj.contains("teamBLogoFile")) {
        teamBLogoFile = obj["teamBLogoFile"].toString();
    }

    teamALogoFile = Utils::localizeFile(teamALogoFile, remoteAssetDir, localAssetDir);
    teamBLogoFile = Utils::localizeFile(teamBLogoFile, remoteAssetDir, localAssetDir);

    swapped = obj["swapped"].toBool();

    for (int i = 0 ; i < 7 && i<allMatches.size(); i++){
        QJsonObject matchObject;

        matchObject = allMatches[i].toObject();
        playerANameList[i] = matchObject["playerA"].toString();
        playerBNameList[i] = matchObject["playerB"].toString();
        playerATimeout[i] = matchObject["playerATimeout"].toBool();
        playerBTimeout[i] = matchObject["playerBTimeout"].toBool();
        playerAServe[i] = matchObject["playerAServe"].toBool();

        QJsonArray gameArray = matchObject["points"].toArray();
        for (int g=0 ; g<5 && g< gameArray.size(); g++)
        {
            QJsonObject result = gameArray[g].toObject();
            points[i][g][0] = result["playerA"].toInt();
            points[i][g][1] = result["playerB"].toInt();
        }
    }

    if (obj.contains("images")) {
        QJsonArray allImages = obj["images"].toArray();
        for (int i = 0 ;  i<allImages.size(); i++){
            QJsonObject imageObj;
            imageObj = allImages[i].toObject();
            QString label = imageObj["label"].toString();
            QString path = imageObj["path"].toString();
            path = Utils::localizeFile(path, remoteAssetDir, localAssetDir);

            imageList[label] = path;
        }
    }

    saveStatus();
    emit dataChanged(index(0,0), index(7,11));
    emit headerDataChanged(Qt::Horizontal, 0, 11);
    emitContentChanges();

    return true;
}

void MatchStatus::addImage(QString label, QString path)
{
    imageList[label] = path;
    saveStatus();

    emitContentChanges();
}

QString MatchStatus::getImage(QString label)
{
    if (imageList.find(label) != imageList.end()) {
        return imageList[label];
    }
    return "";
}

void MatchStatus::saveStatus()
{
    exportInfo(QStringLiteral("./save.json"));
}

void MatchStatus::emitContentChanges()
{
    version += 1;
    emit contentChanged();
}

void MatchStatus::readStatus()
{
    importInfo(QStringLiteral("./save.json"));
}
