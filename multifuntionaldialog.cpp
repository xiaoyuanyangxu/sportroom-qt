#include "multifuntionaldialog.h"
#include "sportroomutils.h"
#include "sportroomutils.h"
#include "ui_multifuntionaldialog.h"
#include <QContextMenuEvent>
#include <QFileDialog>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QtDebug>

MultifuntionalDialog::MultifuntionalDialog(MatchStatus *matchModel,
                                           StateDatamodel *stateModel,
                                           PlayerDatamodel *playerDatamodel,
                                           QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MultifuntionalDialog),
    contextMenu(0)
{
    ui->setupUi(this);

    ui->fullMatchResultWidget->setStatusModel(matchModel);
    ui->playerStatsWidget->setModels(matchModel, playerDatamodel);

    this->matchStatusModel = matchModel;
    this->stateModel       = stateModel;
    this->playerDatamodel  = playerDatamodel;

    QObject::connect(matchStatusModel, &MatchStatus::contentChanged,
                     this, &MultifuntionalDialog::contentChanged);

    QObject::connect(stateModel, &StateDatamodel::contentChanged,
                     this, &MultifuntionalDialog::contentChanged);

    contentChanged();
}

MultifuntionalDialog::~MultifuntionalDialog()
{
    delete ui;
}

void MultifuntionalDialog::contentChanged()
{
    int state = stateModel->getMultifunctionaScreenState();
    QString layer3Image, layer4Image, layer5Image;

    layer3Image = stateModel->getMultifunctionaScreenLayer3Image();
    layer4Image = stateModel->getMultifunctionaScreenLayer4Image();
    layer5Image = stateModel->getMultifunctionaScreenLayer5Image();

    if (!layer3Image.isEmpty())
    {
        SportRoomUtils::drawImage(ui->layer3ImageLabel, layer3Image);
    }
    if (!layer4Image.isEmpty())
    {
        SportRoomUtils::drawImage(ui->layer4ImageLabel, layer4Image);
    }
    if (!layer5Image.isEmpty())
    {
        SportRoomUtils::drawImage(ui->layer5ImageLabel, layer5Image);
    }
    qDebug() << Q_FUNC_INFO << state;

    ui->stackedWidget->setCurrentIndex(state);
    /*
    QGraphicsOpacityEffect * show_effect = new QGraphicsOpacityEffect(ui->stackedWidget);
    QPropertyAnimation* animation = new QPropertyAnimation(show_effect, "opacity");
    ui->stackedWidget->setGraphicsEffect(show_effect);
    animation->setStartValue(1);
    animation->setEndValue(0);
    animation->setDuration(200);*/
}

void MultifuntionalDialog::createContextMenu()
{
    delete(contextMenu);

    contextMenu  = new QMenu(this);

    contextMenu->setStyleSheet("color: black;background-color: white;");

    QAction *changeLayer3Image;
    changeLayer3Image   = new QAction(QObject::tr("Layer 3 Image"),
                             this);
    connect(changeLayer3Image,
            SIGNAL(triggered()),
            this,
            SLOT(changeLayer3ImageSlot()));

    QAction *changeLayer4Image;
    changeLayer4Image   = new QAction(QObject::tr("Layer 4 Image"),
                             this);
    connect(changeLayer4Image,
            SIGNAL(triggered()),
            this,
            SLOT(changeLayer4ImageSlot()));

    QAction *changeLayer5Image;
    changeLayer5Image   = new QAction(QObject::tr("Layer 5 Image"),
                             this);
    connect(changeLayer5Image,
            SIGNAL(triggered()),
            this,
            SLOT(changeLayer5ImageSlot()));

    contextMenu->addAction(changeLayer3Image);
    contextMenu->addAction(changeLayer4Image);
    contextMenu->addAction(changeLayer5Image);
}

void MultifuntionalDialog::changeLayer3ImageSlot()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                   "Select Image File",
                                                   "",
                                                   tr("Images (*.png *.jpeg *.jpg)"));
    qDebug() << Q_FUNC_INFO << fileName;
    if (!fileName.isEmpty())
    {
        stateModel->setMultifunctionaScreenLayer3Image(fileName);
    }
}

void MultifuntionalDialog::changeLayer4ImageSlot()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                   "Select Image File",
                                                   "",
                                                   tr("Images (*.png *.jpeg *.jpg)"));
    qDebug() << Q_FUNC_INFO << fileName;
    if (!fileName.isEmpty())
    {
        stateModel->setMultifunctionaScreenLayer4Image(fileName);
    }
}

void MultifuntionalDialog::changeLayer5ImageSlot()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                   "Select Image File",
                                                   "",
                                                   tr("Images (*.png *.jpeg *.jpg)"));
    qDebug() << Q_FUNC_INFO << fileName;
    if (!fileName.isEmpty())
    {
        stateModel->setMultifunctionaScreenLayer5Image(fileName);
    }
}


void MultifuntionalDialog::resizeEvent(QResizeEvent *event)
{
    SportRoomUtils::storeSize(this, "multifunctional_dialog");
}

void MultifuntionalDialog::contextMenuEvent(QContextMenuEvent *event)
{
    qDebug()<< Q_FUNC_INFO<<event;
    createContextMenu();
    contextMenu->popup(event->globalPos());
}
