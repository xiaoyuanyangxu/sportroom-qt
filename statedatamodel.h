#ifndef STATEDATAMODEL_H
#define STATEDATAMODEL_H

#include <QAbstractTableModel>
#include <QDateTime>
#include <QItemDelegate>
#include <QObject>
#include <QPainter>
#include <QStyleOptionViewItem>

class State
{
public:
    State(){};

    QDateTime startTime;
    QDateTime endTime;

    int startState;
    int newState;
};

class StatusListDelegate : public QItemDelegate
{
    Q_OBJECT

public:
    StatusListDelegate(QList<State> * stateList) {
        this->stateList = stateList;
    }
    // QAbstractItemDelegate interface
public:
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

private:
    QList<State> * stateList;
};

#define STATE_FILE_NAME "./state.json"
class StateDatamodel : public QAbstractTableModel
{
    Q_OBJECT
public:
    StateDatamodel();

    void setCurrentStatus(int status);
    int getCurrentStatus();

    void setLocalUpdateDelay(int d);
    int getLocalUpdateDelay() {return localUpdateDelay;};
    void setGlobalUpdateDelay(int d);
    int getGlobalUpdateDelay() {return globalUpdateDelay;};
    void setLocalUpdateDelaySelected(bool selected);
    bool getLocalUpdateDelaySelected(){return localUpdateDelaySelected;}
    void setMatchSyncPushSelected(bool selected);
    bool getMatchSyncPushSelected() { return matchSyncPushSelected; }

    void setMultifunctionaScreenState(int state);
    int getMultifunctionaScreenState();

    void setElementState(int elementMask, bool enabled);
    bool getElementState(int elementMask);

    void setMultifunctionaScreenLayer3Image(QString path);
    void setMultifunctionaScreenLayer4Image(QString path);
    void setMultifunctionaScreenLayer5Image(QString path);
    QString getMultifunctionaScreenLayer3Image();
    QString getMultifunctionaScreenLayer4Image();

    QString getMultifunctionaScreenLayer5Image();

    int getCurrentVersion(){return version;};

    void reset();

    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    QByteArray exportInfoAsJson();
    int importInfoFromJson(const QByteArray &json, const bool local);

private:
    void saveStatus();
    void readStatus();

    int readContent(QString fileName);
    void saveContent(const QString &fileNam);

    void emitContentChanged();

signals:
    void contentChanged();

private:
    QVector<State> stateList;

    bool localUpdateDelaySelected;
    int localUpdateDelay;
    int globalUpdateDelay;
    bool matchSyncPushSelected;

    int elementState;

    int multifunctionalScreenState;
    QString layer3Image;
    QString layer4Image;
    QString layer5Image;

    int version;
};

#endif // STATEDATAMODEL_H
