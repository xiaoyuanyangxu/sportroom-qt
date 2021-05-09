#ifndef REFLECTORCONNECTOR_H
#define REFLECTORCONNECTOR_H

#include "matchstatus.h"
#include "statedatamodel.h"
#include "playerdatamodel.h"

#include <QObject>
#include <QtWebSockets/QWebSocket>

class DataElement {

public:
    DataElement()
    {

    }
    DataElement(QString type, const QByteArray d)
    {
        this->type = type;
        data = d;
        timestamp = QDateTime::currentDateTime();
    }
    QString type;
    QByteArray data;
    QDateTime timestamp;
};

class ReflectorConnector : public QObject
{
    Q_OBJECT
public:
    explicit ReflectorConnector(
            MatchStatus *matchStatus,
            StateDatamodel * stateDatamodel,
            PlayerDatamodel * playerDataModel,
            QObject *parent = nullptr);

    void connect2Reflector(const QUrl &url);

    void close();

    bool isConnected();

    void getState(bool &connected, bool &reconnecting, bool &closed, QString &id);
    void push();
    void pull();

signals:
    void stateChanged();

private slots:
    void onConnected();
    void onTextMessageReceived(QString message);
    void onClosed();
    void onTimeout();

    void contentChanged();
    void stateContentChanged();
    void playerContentChanged();

    void consolidateData();

public:
    MatchStatus    * matchStatus;
    int              lastReportedMatchStatus;
    StateDatamodel * stateDatamodel;
    int              lastReportedStateDatamodel;
    PlayerDatamodel * playerDatamodel;
    int              lastReportedPlayerDatamodel;


private:
    bool       connected;
    bool       reconnecting;
    bool       closed;
    QString    id;
    QWebSocket webSocket;
    QUrl       url;

    QTimer     * timer;
    QList<DataElement*> queue;
};

#endif // REFLECTORCONNECTOR_H
