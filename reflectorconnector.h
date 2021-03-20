#ifndef REFLECTORCONNECTOR_H
#define REFLECTORCONNECTOR_H

#include "matchstatus.h"
#include "statedatamodel.h"

#include <QObject>
#include <QtWebSockets/QWebSocket>

class ReflectorConnector : public QObject
{
    Q_OBJECT
public:
    explicit ReflectorConnector(
            MatchStatus *matchStatus,
            StateDatamodel * stateDatamodel,
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

public:
    MatchStatus    * matchStatus;
    int              lastReportedMatchStatus;
    StateDatamodel * stateDatamodel;
    int              lastReportedStateDatamodel;

private:
    bool       connected;
    bool       reconnecting;
    bool       closed;
    QString    id;
    QWebSocket webSocket;
    QUrl       url;

    QTimer     * timer;
};

#endif // REFLECTORCONNECTOR_H
