#include "multifuntionaldialog.h"
#include "sportroomutils.h"
#include "sportroomutils.h"
#include "ui_multifuntionaldialog.h"
#include <QContextMenuEvent>
#include <QFileDialog>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QSettings>
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

    changeSize(1280,720);

    QObject::connect(matchStatusModel, &MatchStatus::contentChanged,
                     this, &MultifuntionalDialog::contentChanged);

    QObject::connect(stateModel, &StateDatamodel::contentChanged,
                     this, &MultifuntionalDialog::contentChanged);

    setAttribute(Qt::WA_QuitOnClose, false);

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
    //new Fader(ui->stackedWidget, 0.0, 1.0, 4000);

    QSettings settings;

    QColor colorBack = QColor(settings.value("other_back","#FFFFFF").toString());
    //this->setStyleSheet(QString("background-color : %1;").arg(colorBack.name(QColor::HexRgb)));
    //ui->playerStatsWidget->setStyleSheet(QString("background-color : %1;").arg(colorBack.name(QColor::HexRgb)));

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

    QAction *changeHDSize;
    changeHDSize   = new QAction(QObject::tr("HD: 1280x720"),
                             this);
    connect(changeHDSize,
            SIGNAL(triggered()),
            this,
            SLOT(changeSizeHDSlot()));

    QAction *changeFullHDSize;
    changeFullHDSize   = new QAction(QObject::tr("FullHD: 1920x1080"),
                             this);
    connect(changeFullHDSize,
            SIGNAL(triggered()),
            this,
            SLOT(changeSizeFullHDSlot()));


    contextMenu->addAction(changeLayer3Image);
    contextMenu->addAction(changeLayer4Image);
    contextMenu->addAction(changeLayer5Image);
    contextMenu->addSeparator();
    contextMenu->addAction(changeHDSize);
    contextMenu->addAction(changeFullHDSize);

}

void MultifuntionalDialog::changeLayer3ImageSlot()
{
    QSettings settings;

    QString fileName = QFileDialog::getOpenFileName(this,
                                                   "Select Image File",
                                                   settings.value("asset_folder","").toString(),
                                                   tr("Images (*.png *.jpeg *.jpg)"));
    qDebug() << Q_FUNC_INFO << fileName;
    if (!fileName.isEmpty())
    {
        stateModel->setMultifunctionaScreenLayer3Image(fileName);
    }
}

void MultifuntionalDialog::changeLayer4ImageSlot()
{
    QSettings settings;

    QString fileName = QFileDialog::getOpenFileName(this,
                                                   "Select Image File",
                                                   settings.value("asset_folder","").toString(),
                                                   tr("Images (*.png *.jpeg *.jpg)"));
    qDebug() << Q_FUNC_INFO << fileName;
    if (!fileName.isEmpty())
    {
        stateModel->setMultifunctionaScreenLayer4Image(fileName);
    }
}

void MultifuntionalDialog::changeLayer5ImageSlot()
{
    QSettings settings;

    QString fileName = QFileDialog::getOpenFileName(this,
                                                   "Select Image File",
                                                   settings.value("asset_folder","").toString(),
                                                   tr("Images (*.png *.jpeg *.jpg)"));
    qDebug() << Q_FUNC_INFO << fileName;
    if (!fileName.isEmpty())
    {
        stateModel->setMultifunctionaScreenLayer5Image(fileName);
    }
}

void MultifuntionalDialog::changeSize(int w, int h)
{
    const int padding = 0;

    this->setMaximumWidth(w + padding);
    this->setMinimumWidth(w + padding);
    this->setMinimumHeight(h + padding);
    this->setMaximumHeight(h + padding);

    ui->layer3ImageLabel->setMinimumSize(QSize(w,h));
    ui->layer4ImageLabel->setMinimumSize(QSize(w,h));
    ui->layer5ImageLabel->setMinimumSize(QSize(w,h));
    ui->layer3ImageLabel->setMaximumSize(QSize(w,h));
    ui->layer4ImageLabel->setMaximumSize(QSize(w,h));
    ui->layer5ImageLabel->setMaximumSize(QSize(w,h));

    contentChanged();
}


void MultifuntionalDialog::changeSizeFullHDSlot()
{
    changeSize(1920,1080);
}

void MultifuntionalDialog::changeSizeHDSlot()
{
    changeSize(1280,720);
}

void MultifuntionalDialog::changeSize2KSlot()
{
    changeSize(2048,1080);
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
