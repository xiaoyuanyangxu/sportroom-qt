#include "reflectorconnector.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QTimer>

ReflectorConnector::ReflectorConnector(MatchStatus *matchStatus,
                                       StateDatamodel * stateDatamodel, PlayerDatamodel *playerDataModel,
                                       QObject *parent) : QObject(parent)
{
    this->matchStatus = matchStatus;
    this->stateDatamodel = stateDatamodel;
    this->playerDatamodel = playerDataModel;

    connected = false;
    closed = true;
    lastReportedMatchStatus = -1;
    lastReportedStateDatamodel = -1;
    lastReportedPlayerDatamodel = -1;
    reconnecting = false;

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &ReflectorConnector::onTimeout);
    timer->start(1000);
}

void ReflectorConnector::connect2Reflector(const QUrl &url)
{
    qDebug() << Q_FUNC_INFO << url;
    connect(&webSocket, &QWebSocket::connected, this, &ReflectorConnector::onConnected);
    connect(&webSocket, &QWebSocket::disconnected, this, &ReflectorConnector::onClosed);

    this->url = url;
    webSocket.open(QUrl(url));

    closed = false;
}


void ReflectorConnector::close()
{
    qDebug() << Q_FUNC_INFO;
    closed = true;
    if (connected)
    {
        webSocket.close();
    }else{
        emit stateChanged();
    }
}

bool ReflectorConnector::isConnected()
{
    return connected;
}

void ReflectorConnector::getState(bool &connected, bool &reconnecting, bool &closed, QString &id)
{
    connected    = this->connected;
    id           = this->id;
    reconnecting = this->reconnecting;
    closed       = this->closed;
}

void ReflectorConnector::push()
{
    contentChanged();
    stateContentChanged();
    playerContentChanged();
}

void ReflectorConnector::pull()
{
    if (connected)
    {
        webSocket.sendTextMessage(QStringLiteral("{\"type\": \"pull\"}"));
    }
}

void ReflectorConnector::onConnected()
{
    connect(&webSocket, &QWebSocket::textMessageReceived,
            this, &ReflectorConnector::onTextMessageReceived);
    //webSocket.sendTextMessage(QString("{\"type\": \"control\", \"id\":\"%1\"}").arg(this->id));
    connected = true;
    reconnecting = false;
    connect(matchStatus, &MatchStatus::contentChanged, this, &ReflectorConnector::contentChanged);
    connect(stateDatamodel, &StateDatamodel::contentChanged, this, &ReflectorConnector::stateContentChanged);
    connect(playerDatamodel, &PlayerDatamodel::contentChanged, this, &ReflectorConnector::playerContentChanged);
    emit stateChanged();
}

void ReflectorConnector::onTextMessageReceived(QString message)
{
    QJsonDocument loadDoc(QJsonDocument::fromJson(message.toUtf8()));
    QJsonObject obj = loadDoc.object();

    qDebug() << Q_FUNC_INFO << obj["type"];
    if (obj["type"] == "control") {
        id = obj["id"].toString();
        emit stateChanged();

    }else if (obj["type"] == "matchStatus") {
        QJsonObject match = obj["content"].toObject();
        QJsonDocument saveDoc(match);

        queue.append(new DataElement(QString("matchStatus"), saveDoc.toJson()));

        //lastReportedMatchStatus = matchStatus->getCurrentVersion() + 1;
        //matchStatus->importInfoFromJson(saveDoc.toJson());
    }else if (obj["type"] == "state") {
        QJsonObject state = obj["content"].toObject();
        QJsonDocument saveDoc(state);

        queue.append(new DataElement(QString("state"), saveDoc.toJson()));

        //lastReportedStateDatamodel = stateDatamodel->getCurrentVersion() + 1;
        //stateDatamodel->importInfoFromJson(saveDoc.toJson());
        lastReportedStateDatamodel = stateDatamodel->getCurrentVersion() + 1;
        stateDatamodel->importInfoFromJson(saveDoc.toJson());
    }else if (obj["type"] == "player") {
        QJsonObject state = obj["content"].toObject();
        QJsonDocument saveDoc(state);

        queue.append(new DataElement(QString("player"), saveDoc.toJson()));

        //lastReportedPlayerDatamodel = playerDatamodel->getCurrentVersion() + 1;
        //playerDatamodel->importInfoFromJson(saveDoc.toJson());
    }else if (obj["type"] == "pull") {
        push();
    }
    consolidateData();
}

void ReflectorConnector::onClosed()
{
    qDebug() << Q_FUNC_INFO;

    if (connected)
    {
        disconnect(matchStatus, &MatchStatus::contentChanged, this, &ReflectorConnector::contentChanged);
        disconnect(stateDatamodel, &StateDatamodel::contentChanged, this, &ReflectorConnector::stateContentChanged);
        disconnect(playerDatamodel, &PlayerDatamodel::contentChanged, this, &ReflectorConnector::playerContentChanged);

        disconnect(&webSocket, &QWebSocket::textMessageReceived,
                this, &ReflectorConnector::onTextMessageReceived);
    }

    disconnect(&webSocket, &QWebSocket::connected, this, &ReflectorConnector::onConnected);
    disconnect(&webSocket, &QWebSocket::disconnected, this, &ReflectorConnector::onClosed);

    connected = false;
    reconnecting = false;

    emit stateChanged();
}

void ReflectorConnector::onTimeout()
{
    qDebug() << Q_FUNC_INFO << closed << connected << reconnecting;
    consolidateData();
    if (closed) {
        return;
    }
    if (connected) {
        return;
    }
    if (reconnecting) {
        return;
    }
    reconnecting = true;
    connect2Reflector(url);
}

void ReflectorConnector::contentChanged()
{
    if (connected)
    {
        int version = matchStatus->getCurrentVersion();

        qDebug() << Q_FUNC_INFO << "version: " << version << " vs last:" << lastReportedMatchStatus;
        if (version != lastReportedMatchStatus)
        {
            lastReportedMatchStatus = version;
            QString body = QString("{\"type\":\"matchStatus\", \"content\":%1}").arg(
                                        QString(matchStatus->exportInfoAsJson()));
            //qDebug() << Q_FUNC_INFO << body;
            webSocket.sendTextMessage(body);
        }
    }
}

void ReflectorConnector::stateContentChanged()
{
    if (connected)
    {
        int version = stateDatamodel->getCurrentVersion();
        if (version != lastReportedStateDatamodel)
        {
            lastReportedStateDatamodel = version;
            QString body = QString("{\"type\":\"state\", \"content\":%1}").arg(
                                        QString(stateDatamodel->exportInfoAsJson()));
            //qDebug() << Q_FUNC_INFO << body;
            qDebug() << Q_FUNC_INFO << "emit content changes";
            webSocket.sendTextMessage(body);
        }
    }
}

void ReflectorConnector::playerContentChanged()
{
    qDebug()<<Q_FUNC_INFO << lastReportedPlayerDatamodel;
    if (connected)
    {
        int version = playerDatamodel->getCurrentVersion();
        if (version != lastReportedPlayerDatamodel)
        {
            lastReportedPlayerDatamodel = version;
            QString body = QString("{\"type\":\"player\", \"content\":%1}").arg(
                                        QString(playerDatamodel->exportInfoAsJson()));
            //qDebug() << Q_FUNC_INFO << body;
            qDebug() << Q_FUNC_INFO << "emit content changes";
            webSocket.sendTextMessage(body);
        }
    }
}

void ReflectorConnector::consolidateData()
{
    while(queue.size())
    {
        QDateTime now = QDateTime::currentDateTime();
        DataElement * p = queue.first();
        int t = p->timestamp.msecsTo(now) / 1000;

        if (t >= matchStatus->getUpdateDelay())
        {
            if (p->type == "matchStatus") {
                lastReportedMatchStatus = matchStatus->getCurrentVersion() + 1;
                matchStatus->importInfoFromJson(p->data, false);
            }else if (p->type == "state") {
                lastReportedStateDatamodel = stateDatamodel->getCurrentVersion() + 1;
                stateDatamodel->importInfoFromJson(p->data);
            }else if (p->type == "player") {
                lastReportedPlayerDatamodel = playerDatamodel->getCurrentVersion() + 1;
                playerDatamodel->importInfoFromJson(p->data);
            }else{

            }
            queue.pop_front();
            delete p;

        }
    }
}

