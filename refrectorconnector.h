#ifndef REFRECTORCONNECTOR_H
#define REFRECTORCONNECTOR_H

#include "matchstatus.h"
#include "statedatamodel.h"

#include <QObject>
#include <QtWebSockets/QWebSocket>

class RefrectorConnector : public QObject
{
    Q_OBJECT
public:
    explicit RefrectorConnector(
            MatchStatus *matchStatus,
            StateDatamodel * stateDatamodel,
            QObject *parent = nullptr);

    void connectAsListener(const QUrl &url);
    void connectAsPublisher(const QUrl &url);

    void close();

    bool isConnected();

    void getState(bool &connected, bool &listenerMode);

signals:
    void stateChanged();

private slots:
    void onConnected();
    void onTextMessageReceived(QString message);
    void onClosed();

    void contentChanged();
    void stateContentChanged();

public:
    MatchStatus    * matchStatus;
    StateDatamodel * stateDatamodel;

private:
    bool       listenerMode;
    bool       connected;
    QWebSocket webSocket;
    QUrl       url;
};

#endif // REFRECTORCONNECTOR_H
