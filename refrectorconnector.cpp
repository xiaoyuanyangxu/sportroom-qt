#include "refrectorconnector.h"

#include <QJsonDocument>
#include <QJsonObject>

RefrectorConnector::RefrectorConnector(MatchStatus *matchStatus,
                                       StateDatamodel * stateDatamodel,
                                       QObject *parent) : QObject(parent)
{
    this->matchStatus = matchStatus;
    this->stateDatamodel = stateDatamodel;
    connected = false;
}

void RefrectorConnector::connectAsListener(const QUrl &url)
{
    connect(&webSocket, &QWebSocket::connected, this, &RefrectorConnector::onConnected);
    connect(&webSocket, &QWebSocket::disconnected, this, &RefrectorConnector::onClosed);
    webSocket.open(QUrl(url));
    listenerMode = true;
}

void RefrectorConnector::connectAsPublisher(const QUrl &url)
{
    connect(&webSocket, &QWebSocket::connected, this, &RefrectorConnector::onConnected);
    connect(&webSocket, &QWebSocket::disconnected, this, &RefrectorConnector::onClosed);
    webSocket.open(QUrl(url));
    listenerMode = false;
}

void RefrectorConnector::close()
{
    qDebug() << Q_FUNC_INFO;
    webSocket.close();
}

bool RefrectorConnector::isConnected()
{
    return connected;
}

void RefrectorConnector::getState(bool &connected, bool &listenerMode)
{
    connected = this->connected;
    listenerMode = this->listenerMode;
}

void RefrectorConnector::onConnected()
{
    connect(&webSocket, &QWebSocket::textMessageReceived,
            this, &RefrectorConnector::onTextMessageReceived);
    webSocket.sendTextMessage(QStringLiteral("{\"type\": \"control\"}"));
    connected = true;
    if(!listenerMode) {
        connect(matchStatus, &MatchStatus::contentChanged, this, &RefrectorConnector::contentChanged);
        connect(stateDatamodel, &StateDatamodel::contentChanged, this, &RefrectorConnector::stateContentChanged);
    }
    emit stateChanged();
}

void RefrectorConnector::onTextMessageReceived(QString message)
{
    if (!listenerMode) {
        QJsonDocument loadDoc(QJsonDocument::fromJson(message.toLocal8Bit()));
        QJsonObject obj = loadDoc.object();

        qDebug() << Q_FUNC_INFO << message;
        if (obj["type"] == "control") {

        }else if (obj["type"] == "matchStatus") {
            matchStatus->importInfoFromJson(obj["content"].toString().toLocal8Bit());
        }else if (obj["type"] == "state") {
            stateDatamodel->importInfoFromJson(obj["content"].toString().toLocal8Bit());
        }
    }
}

void RefrectorConnector::onClosed()
{
    qDebug() << Q_FUNC_INFO;
    connected = false;
    if (!listenerMode) {
        disconnect(matchStatus, &MatchStatus::contentChanged, this, &RefrectorConnector::contentChanged);
        disconnect(stateDatamodel, &StateDatamodel::contentChanged, this, &RefrectorConnector::stateContentChanged);
    }
    emit stateChanged();
}

void RefrectorConnector::contentChanged()
{
    if (!listenerMode && connected)
    {
        QString body = QString("{\"type\":\"matchStatus\", \"content\":%1}").arg(
                                    QString(matchStatus->exportInfoAsJson()));
        qDebug() << Q_FUNC_INFO << body;
        webSocket.sendTextMessage(body);
    }
}

void RefrectorConnector::stateContentChanged()
{
    if (!listenerMode && connected)
    {
        QString body = QString("{\"type\":\"state\", \"content\":%1}").arg(
                                    QString(stateDatamodel->exportInfoAsJson()));
        qDebug() << Q_FUNC_INFO << body;
        webSocket.sendTextMessage(body);
    }
}
