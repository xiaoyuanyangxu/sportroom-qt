#ifndef FULLMATCHRESULTFORM_H
#define FULLMATCHRESULTFORM_H

#include "matchstatus.h"

#include <QLabel>
#include <QMenu>
#include <QPushButton>
#include <QWidget>

namespace Ui {
class FullMatchResultForm;
}

class FullMatchResultForm : public QWidget
{
    Q_OBJECT

public:
    explicit FullMatchResultForm(QWidget *parent = nullptr);
    ~FullMatchResultForm();

    void setStatusModel(MatchStatus * statusModel);

private:
    void drawImage(QLabel *label, QString uri);
    void createContextMenu();
    bool changeImageIcon(QPushButton *pushButton,  QString fileName, int maxWidth=0);
    bool setImage(QString label);
    void drawAllImages();

public slots:
    void contentChanged();

private slots:
    void switchingMode();

    void on_imageTopLeftPushButton_clicked();

    void on_imageTopRightPushButton_clicked();

    void on_imageBottomLeftPushButton_clicked();

    void on_imageBottomRightPushButton_clicked();

private:
    Ui::FullMatchResultForm *ui;

    MatchStatus             *statusModel;

    bool                    editingMode;
    QMenu                   *contextMenu;

    // QWidget interface
protected:
    void contextMenuEvent(QContextMenuEvent *event);

    // QWidget interface
protected:
    void resizeEvent(QResizeEvent *event);
};

#endif // FULLMATCHRESULTFORM_H
