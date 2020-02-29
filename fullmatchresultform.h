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
    void createContextMenu();
    bool changeImageIcon(QPushButton *pushButton,  QString fileName, int maxWidth, int maxHeight);
    bool setImage(QString label);
    void drawTeamLogoImages();
    void drawBottomImages(int maxWidth, int maxHeight);
    void drawTopImages(int maxWidth, int maxHeight);
    void doResize(QSize size);

public slots:
    void contentChanged();

private slots:
    void backgroundColor();

    void changeTopLeftImage();
    void changeTopRightImage();
    void changeBottomLeftImage();
    void changeBottomRightImage();

    void saveToAImage();

private:
    Ui::FullMatchResultForm *ui;

    MatchStatus             *statusModel;
    QMenu                   *contextMenu;
    QString                  imageFilePath;


    // QWidget interface
protected:
    void contextMenuEvent(QContextMenuEvent *event);

    // QWidget interface
protected:
    void resizeEvent(QResizeEvent *event);
};

#endif // FULLMATCHRESULTFORM_H
