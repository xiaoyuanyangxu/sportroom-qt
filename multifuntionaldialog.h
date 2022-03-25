#ifndef MULTIFUNTIONALDIALOG_H
#define MULTIFUNTIONALDIALOG_H

#include "matchstatus.h"
#include "playerdatamodel.h"
#include "statedatamodel.h"

#include <QDialog>
#include <QMenu>

namespace Ui {
class MultifuntionalDialog;
}

class MultifuntionalDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MultifuntionalDialog(MatchStatus * matchModel,
                                  StateDatamodel * stateModel,
                                  PlayerDatamodel * playerDatamodel,
                                  QWidget *parent = nullptr);
    ~MultifuntionalDialog();

public slots:
    void contentChanged();

private:
    void createContextMenu();
    void changeSize(int w, int h);

private slots:
    void changeLayer3ImageSlot();
    void changeLayer4ImageSlot();
    void changeLayer5ImageSlot();
    void changeSizeFullHDSlot();
    void changeSizeHDSlot();
    void changeSize2KSlot();


private:
    Ui::MultifuntionalDialog *ui;

    // QWidget interface
protected:
    void resizeEvent(QResizeEvent *event);
    void contextMenuEvent(QContextMenuEvent *event);

private:
    MatchStatus *           matchStatusModel;
    StateDatamodel *        stateModel;
    PlayerDatamodel*        playerDatamodel;

    QString                 image1Path;
    QString                 image2Path;

    QMenu*                  contextMenu;

};

#endif // MULTIFUNTIONALDIALOG_H
