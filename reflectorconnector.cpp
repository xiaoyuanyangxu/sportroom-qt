#include "reflectorconnector.h"

#include <QJsonDocument>
#include <QJsonObject>

ReflectorConnector::ReflectorConnector(MatchStatus *matchStatus,
                                       StateDatamodel * stateDatamodel,
                                       QObject *parent) : QObject(parent)
{
    this->matchStatus = matchStatus;
    this->stateDatamodel = stateDatamodel;
    connected = false;
}

void ReflectorConnector::connect2Reflector(const QUrl &url)
{
    connect(&webSocket, &QWebSocket::connected, this, &ReflectorConnector::onConnected);
    connect(&webSocket, &QWebSocket::disconnected, this, &ReflectorConnector::onClosed);
    webSocket.open(QUrl(url));
}


void ReflectorConnector::close()
{
    qDebug() << Q_FUNC_INFO;
    webSocket.close();
}

bool ReflectorConnector::isConnected()
{
    return connected;
}

void ReflectorConnector::getState(bool &connected, QString &id)
{
    connected = this->connected;
    id        = this->id;
}

void ReflectorConnector::push()
{
    contentChanged();
    stateChanged();
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
    webSocket.sendTextMessage(QStringLiteral("{\"type\": \"control\"}"));
    connected = true;
    connect(matchStatus, &MatchStatus::contentChanged, this, &ReflectorConnector::contentChanged);
    connect(stateDatamodel, &StateDatamodel::contentChanged, this, &ReflectorConnector::stateContentChanged);
    emit stateChanged();
}

void ReflectorConnector::onTextMessageReceived(QString message)
{
    QJsonDocument loadDoc(QJsonDocument::fromJson(message.toLocal8Bit()));
    QJsonObject obj = loadDoc.object();

    //qDebug() << Q_FUNC_INFO << message;
    if (obj["type"] == "control") {
        id = obj["id"].toString();
        emit stateChanged();

    }else if (obj["type"] == "matchStatus") {
        QJsonObject match = obj["content"].toObject();
        QJsonDocument saveDoc(match);

        matchStatus->importInfoFromJson(saveDoc.toJson());
    }else if (obj["type"] == "state") {
        QJsonObject state = obj["content"].toObject();
        QJsonDocument saveDoc(state);

        stateDatamodel->importInfoFromJson(saveDoc.toJson());
    }else if (obj["type"] == "pull") {
        push();
    }
}

void ReflectorConnector::onClosed()
{
    qDebug() << Q_FUNC_INFO;
    connected = false;
    disconnect(matchStatus, &MatchStatus::contentChanged, this, &ReflectorConnector::contentChanged);
    disconnect(stateDatamodel, &StateDatamodel::contentChanged, this, &ReflectorConnector::stateContentChanged);
    emit stateChanged();
}

void ReflectorConnector::contentChanged()
{
    if (connected)
    {
        QString body = QString("{\"type\":\"matchStatus\", \"content\":%1}").arg(
                                    QString(matchStatus->exportInfoAsJson()));
        //qDebug() << Q_FUNC_INFO << body;
        webSocket.sendTextMessage(body);
    }
}

void ReflectorConnector::stateContentChanged()
{
    if (connected)
    {
        QString body = QString("{\"type\":\"state\", \"content\":%1}").arg(
                                    QString(stateDatamodel->exportInfoAsJson()));
        //qDebug() << Q_FUNC_INFO << body;
        webSocket.sendTextMessage(body);
    }
}
