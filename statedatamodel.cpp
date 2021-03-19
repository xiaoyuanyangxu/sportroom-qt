#include "statedatamodel.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>

StateDatamodel::StateDatamodel()
{
    readStatus();
}

int StateDatamodel::readContent(QString fileName)
{
    qDebug() << Q_FUNC_INFO << fileName;

    QFile loadFile(fileName);

    if (!loadFile.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open save file.");
        return -1;
    }

    QByteArray saveData = loadFile.readAll();

    return importInfoFromJson(saveData);
}

void StateDatamodel::saveContent(const QString &fileName)
{
    qDebug() << Q_FUNC_INFO << fileName;

    QFile saveFile(fileName);
    QJsonObject doc;

    if (!saveFile.open(QIODevice::WriteOnly)) {
        qWarning("Couldn't open save file.");
        return;
    }

    saveFile.write(exportInfoAsJson());
}


void StateDatamodel::setCurrentStatus(int status)
{

    qDebug() << Q_FUNC_INFO << status << " size:" << stateList.size();
    status = status & 0x0F;

    QDateTime startTime = QDateTime::currentDateTime();
    QDateTime endTime   = startTime;
    int startState = 0;

    State newState;

    if (!stateList.isEmpty()) {
        State lastState = stateList.first();
        if (lastState.newState == status){
            return;
        }
        startTime = lastState.endTime;
        startState = lastState.newState;
    }

    newState.startState = startState;
    newState.endTime    = endTime;
    newState.newState   = status;
    newState.startTime  = startTime;

    beginInsertRows(QModelIndex(), 0, 0);
    stateList.insert(0, newState);
    endInsertRows();

    saveStatus();

    emit dataChanged(QModelIndex(), index( stateList.size(),5));
    emit contentChanged();
}

int StateDatamodel::getCurrentStatus()
{
    qDebug() << Q_FUNC_INFO;
    if (stateList.isEmpty()) {
        return 0;
    }
    State lastState = stateList.first();
    return lastState.newState;
}

int StateDatamodel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    //qDebug() << Q_FUNC_INFO << stateList.size();
    return stateList.size();
}

int StateDatamodel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 5;
}

QVariant StateDatamodel::data(const QModelIndex &index, int role) const
{
    //qDebug() << Q_FUNC_INFO << index << " role:" << role;
    if (role == Qt::DisplayRole)
    {
        QString text;

        int row = index.row();
        int col = index.column();

        State state = stateList.at(row);
        int sec, ms;
        switch(col) {
        case 0:
            text = state.startTime.toString(Qt::ISODateWithMs);
            break;
        case 1:
            text = state.endTime.toString(Qt::ISODateWithMs);
            break;
        case 2:
            ms = state.startTime.msecsTo(state.endTime);
            if (ms>1000) {
                sec = ms/1000;
                ms = ms % 1000;
            }else{
                sec = 0;
            }
            if (sec>0) {
                text = QString("%1.%2 sec").arg(sec).arg(ms);
            }else{
                text = QString("%1 ms").arg(ms);
            }
            break;
        case 3:
            text = QString::number(state.startState);
            break;
        case 4:
            text = QString::number(state.newState);
            break;
        }
        return text;
    }

    if (role == Qt::BackgroundRole)
    {

        int row = index.row();

        State state = stateList.at(row);

        switch(state.startState)
        {
        case 1:
            return QBrush(Qt::white);
            break;
        case 2:
            return QBrush(Qt::green);
            break;
        case 4:
            return QBrush(Qt::lightGray);
            break;
        case 8:
            return QBrush(Qt::yellow);
            break;
        }
    }
    return QVariant();
}

QVariant StateDatamodel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if ((role == Qt::DisplayRole) && (orientation == Qt::Horizontal))
    {
        switch(section)
        {
        case 0:
            return tr("StartTime");
        case 1:
            return tr("EndTime");
        case 2:
            return tr("Duration");;
        case 3:
            return tr("State");
        case 4:
            return tr("NewState");
        default:
            return QVariant();
        }
    }
    return QVariant();
}

QByteArray StateDatamodel::exportInfoAsJson()
{
    QJsonArray statusList;

    for (auto it = stateList.begin();it!= stateList.end(); it++)
    {
        QJsonObject status;
        status["startTime"] = it->startTime.toString(Qt::ISODateWithMs);
        status["endTime"] = it->endTime.toString(Qt::ISODateWithMs);
        status["startState"] = it->startState;
        status["newState"] = it->newState;
        statusList.append(status);
    }
    QJsonObject doc;
    doc["statusList"] = statusList;

    QJsonDocument saveDoc(doc);
    return saveDoc.toJson();
}

int StateDatamodel::importInfoFromJson(const QByteArray &json)
{
    QVector<State> newStateList;
    QJsonDocument loadDoc(QJsonDocument::fromJson(json));

    QJsonObject obj = loadDoc.object();

    QJsonArray statusList = obj["statusList"].toArray();
    for (int i = 0 ;  i<statusList.size(); i++){
        QJsonObject statusObj;
        State state;
        statusObj = statusList[i].toObject();
        state.startTime = QDateTime::fromString(statusObj["startTime"].toString(), Qt::ISODateWithMs);
        state.endTime = QDateTime::fromString(statusObj["endTime"].toString(), Qt::ISODateWithMs);
        state.startState = statusObj["startState"].toInt();
        state.newState = statusObj["newState"].toInt();

        newStateList.append(state);
    }

    beginInsertRows(QModelIndex(), 0,newStateList.size());
    newStateList.swap(stateList);
    endInsertRows();

    saveStatus();

    emit dataChanged(index(0,0), index(this->stateList.size(),5));
    emit headerDataChanged(Qt::Horizontal, 0, 5);
    emit contentChanged();

    return this->stateList.size();
}

void StateDatamodel::saveStatus()
{
    saveContent(STATE_FILE_NAME);
}

void StateDatamodel::readStatus()
{
    readContent(STATE_FILE_NAME);
}

void StatusListDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QColor fillColor = option.palette.color(QPalette::Background);
    int flags = Qt::AlignCenter|Qt::TextWordWrap;

    int col = index.column();
    int row = index.row();
    QString text;

    if (row >= stateList->size())
    {
        return;
    }
    State state = stateList->at(row);
    switch(col) {
    case 1:
        text = state.startTime.toString(Qt::ISODateWithMs);
        break;
    case 2:
        text = state.endTime.toString(Qt::ISODateWithMs);
        break;
    case 3:
        text = QString::number(state.startState);
        break;
    case 4:
        text = QString::number(state.newState);
        break;
    }


    painter->fillRect(option.rect, fillColor);

    QRect r;
    r = option.rect.adjusted(1, 0, -1, 0);

    painter->drawText(r.left(),
                  r.top(),
                  r.width(),
                  r.height(),
                  flags, text, &r);
}
