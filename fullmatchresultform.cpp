#include "fullmatchresultform.h"
#include "ui_fullmatchresultform.h"

#include <QDate>
#include <QImageReader>
#include <QDebug>
#include <QContextMenuEvent>
#include <QFileDialog>
#include <QColorDialog>
#include <QTimer>
#include "sportroomutils.h"

FullMatchResultForm::FullMatchResultForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FullMatchResultForm),
    contextMenu(0)
{
    ui->setupUi(this);
    imageFilePath = "";

    update();

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, QOverload<>::of(&FullMatchResultForm::update));
    timer->start(1000);
}

FullMatchResultForm::~FullMatchResultForm()
{
    delete contextMenu;
    delete ui;
    //delete timer;
}

void FullMatchResultForm::setStatusModel(MatchStatus *statusModel)
{

    this->statusModel = statusModel;
    ui->matchResultTableView->setModel(statusModel);
    ui->scoreboardWidget->setStatusModel(statusModel);

    ui->matchResultTableView->verticalHeader()->hide();
    ui->matchResultTableView->setShowGrid(true);

    ui->matchResultTableView->setSelectionMode(QAbstractItemView::NoSelection);
    ui->matchResultTableView->setSelectionBehavior(QAbstractItemView::SelectItems);
    ui->matchResultTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);


    ui->matchResultTableView->setColumnWidth(0, 20);
    ui->matchResultTableView->setColumnWidth(2, 20);

    for (int i = 4; i <=10 ; i++){
       ui->matchResultTableView->setColumnWidth(i, 60);
    }

    QHeaderView *header = ui->matchResultTableView->horizontalHeader();
    header->setSectionResizeMode(1, QHeaderView::Stretch);
    header->setSectionResizeMode(3, QHeaderView::Stretch);


    QHeaderView *verticalHeader = ui->matchResultTableView->verticalHeader();
    verticalHeader->sectionResizeMode(QHeaderView::Fixed);
    verticalHeader->setDefaultSectionSize(26);


    QObject::connect(statusModel, &MatchStatus::contentChanged,
                     this, &FullMatchResultForm::contentChanged);
    contentChanged();

}

void FullMatchResultForm::createContextMenu()
{
    delete(contextMenu);

    contextMenu  = new QMenu(this);

    contextMenu->setStyleSheet("color: black;background-color: white;");

    QAction *changeTopLeft;
    changeTopLeft   = new QAction(QObject::tr("Change Top Left Image"),
                             this);
    connect(changeTopLeft,
            SIGNAL(triggered()),
            this,
            SLOT(changeTopLeftImage()));

    QAction *changeTopRight;
    changeTopRight   = new QAction(QObject::tr("Change Top Right Image"),
                             this);
    connect(changeTopRight,
            SIGNAL(triggered()),
            this,
            SLOT(changeTopRightImage()));

    QAction *changeBottomLeft;
    changeBottomLeft   = new QAction(QObject::tr("Change Bottom Left Image"),
                             this);
    connect(changeBottomLeft,
            SIGNAL(triggered()),
            this,
            SLOT(changeBottomLeftImage()));

    QAction *changeBottomRight;
    changeBottomRight   = new QAction(QObject::tr("Change Bottom Right Image"),
                             this);
    connect(changeBottomRight,
            SIGNAL(triggered()),
            this,
            SLOT(changeBottomRightImage()));

    QAction *saveToAImageAction;
    saveToAImageAction   = new QAction(QObject::tr("Save to a Image"),
                             this);
    connect(saveToAImageAction,
            SIGNAL(triggered()),
            this,
            SLOT(saveToAImage()));


    contextMenu->addAction(changeTopLeft);
    contextMenu->addAction(changeTopRight);
    contextMenu->addAction(changeBottomLeft);
    contextMenu->addAction(changeBottomRight);
    contextMenu->addSeparator();
    contextMenu->addAction(saveToAImageAction);

}

bool FullMatchResultForm::changeImageIcon(QPushButton *pushButton, QString fileName, int maxWidth, int maxHeight)
{
    if (!fileName.isEmpty())
    {
        int targetHeight = pushButton->size().height();

        QImageReader imageReader(fileName);
        QSize imageSize = imageReader.size();
        int targetWidth = imageSize.width() * ((float)targetHeight/imageSize.height());

        if (maxWidth && (targetWidth > maxWidth)) {
            //qDebug() << Q_FUNC_INFO << "too big " << targetWidth << " " << maxWidth;
            targetWidth = maxWidth;
            targetHeight = imageSize.height() *((float)targetWidth / imageSize.width());
            //qDebug() << Q_FUNC_INFO << targetHeight;
        }

        if (maxHeight && targetHeight > maxHeight)
        {
            targetHeight = maxHeight;
            targetWidth  = imageSize.width() * ((float)targetHeight / imageSize.height());
        }

        imageReader.setScaledSize(QSize(targetWidth, targetHeight));
        if (!imageReader.canRead())
        {
            return false;
        }
        QImage image = imageReader.read();
        QPixmap pixmap = QPixmap::fromImage(image);

        // set a scaled pixmap to a w x h window keeping its aspect ratio
        // pushButton->setSizeIncrement(targetWidth, height);
        pushButton->setIconSize(QSize(targetWidth, targetHeight));
        pushButton->setIcon( QIcon(pixmap));

        return true;
    }
    return false;
}

bool FullMatchResultForm::setImage(QString label)
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                   "Select Image File",
                                                   "",
                                                   tr("Images (*.png *.png)"));

    if (!fileName.isEmpty())
    {
        statusModel->addImage(label, fileName);
        return true;
    }
    return false;
}


void FullMatchResultForm::contentChanged()
{
    QString teamA, teamB;
    teamA = statusModel->getTeamAName();
    teamB = statusModel->getTeamBName();

    ui->teamANameLabel->setText(teamA);
    ui->teamBNameLabel->setText(teamB);
    int teamAResult, teamBResult;

    statusModel->getCurrentResult(teamAResult, teamBResult);

    ui->teamAPointsLabel->setText(QString::number(teamAResult));
    ui->teamBPointsLabel->setText(QString::number(teamBResult));

    SportRoomUtils::drawImage(ui->teamALogoLabel, statusModel->getTeamALogoFile());
    SportRoomUtils::drawImage(ui->teamBLogoLabel, statusModel->getTeamBLogoFile());

    int currentGame, currentMatch;
    int playerAPoint, playerBPoint;
    int playerAGamePoint, playerBGamePoint;

    QString playerAName, playerBName;

    statusModel->getCurrentMatch(currentGame, currentMatch);

    statusModel->getPoints(currentGame, currentMatch, playerAPoint, playerBPoint);
    statusModel->getPlayerName(currentGame, playerAName, playerBName);

    statusModel->getCurrentGameResult(currentGame, playerAGamePoint, playerBGamePoint);

    doResize(this->size());
}

void FullMatchResultForm::update()
{
    QDateTime now = QDateTime::currentDateTime();
    ui->dayLabel->setText(now.toString("hh:mm:ss dd/MM/yyyy"));
}


void FullMatchResultForm::backgroundColor()
{
    QColorDialog *dialog = new QColorDialog(this);

    dialog->setModal(true);

    dialog->show();
    connect(dialog,
            &QColorDialog::finished,
            [=](int result){
                if (result)
                {
                        qDebug() << "Color Selected" << dialog->getColor();
                        Q_UNUSED(result);
                        QColor color = dialog->getColor();
                        int r,g,b;
                        color.getRgb(&r,&g,&b);
                        this->setStyleSheet(QString("background-color: rgb(%1, %2, %3);").arg(r).arg(g).arg(b));
                        dialog->hide();
                        dialog->deleteLater();
                }
    });




}

void FullMatchResultForm::changeTopLeftImage()
{
    if (setImage("topLeft"))
    {
        ui->imageTopLeftPushButton->setText("");
    }
}

void FullMatchResultForm::changeTopRightImage()
{
    if (setImage("topRight"))
    {
        ui->imageTopRightPushButton->setText("");
    }
}

void FullMatchResultForm::changeBottomLeftImage()
{
    if (setImage("bottomLeft"))
    {
        ui->imageBottomLeftPushButton->setText("");
    }
}

void FullMatchResultForm::changeBottomRightImage()
{
    if (setImage("bottomRight"))
    {
        ui->imageBottomRightPushButton->setText("");
    }
}

void FullMatchResultForm::saveToAImage()
{
    if (imageFilePath.isEmpty())
    {
        QString fileName = QFileDialog::getSaveFileName(this,
                                                       "Select Image File",
                                                       "",
                                                       tr("Images (*.png *.png)"));

        if (!fileName.isEmpty())
        {
            imageFilePath = fileName;
        }else{
            return;
        }
    }
    QPixmap pixmap(this->size());

    this->render(&pixmap);

    pixmap.save(imageFilePath);
}

void FullMatchResultForm::contextMenuEvent(QContextMenuEvent *event)
{
    qDebug()<< Q_FUNC_INFO<<event;
    createContextMenu();
    contextMenu->popup(event->globalPos());
}


void FullMatchResultForm::resizeEvent(QResizeEvent *event)
{
    qDebug() << Q_FUNC_INFO << event->size();

    doResize(event->size());
}


void FullMatchResultForm::drawTeamLogoImages()
{
    SportRoomUtils::drawImage(ui->teamALogoLabel, statusModel->getTeamALogoFile());
    SportRoomUtils::drawImage(ui->teamBLogoLabel, statusModel->getTeamBLogoFile());
}

void FullMatchResultForm::drawBottomImages(int maxWidth, int maxHeight)
{
    QString path = statusModel->getImage("bottomRight");
    this->changeImageIcon(ui->imageBottomRightPushButton, path, maxWidth, maxHeight);
    path = statusModel->getImage("bottomLeft");
    this->changeImageIcon(ui->imageBottomLeftPushButton, path, maxWidth, maxHeight);

}

void FullMatchResultForm::drawTopImages(int maxWidth, int maxHeight)
{
    QString path = statusModel->getImage("topLeft");
    this->changeImageIcon(ui->imageTopLeftPushButton, path, maxWidth, maxHeight);
    path = statusModel->getImage("topRight");
    this->changeImageIcon(ui->imageTopRightPushButton, path, maxWidth, maxHeight);
}

void FullMatchResultForm::doResize(QSize size)
{
    qDebug() << Q_FUNC_INFO << size;
    int topHeight, bottomHeight;

    topHeight = size.height() * 0.20 - ui->dayLabel->height();
    bottomHeight = size.height() * 0.30;

    ui->teamResultWidget->setMinimumHeight(topHeight);
    ui->playerResultWidget->setMinimumHeight(bottomHeight);
    ui->teamResultWidget->setMaximumHeight(topHeight);
    ui->playerResultWidget->setMaximumHeight(bottomHeight);


    float fontSize;
    float tableFontSize;
    int logoSize;
    int colSize = 20;
    QFont f;

    fontSize = 16;
    logoSize = 30;
    tableFontSize = 13;

    if (size.width() >= 620 && size.height() >= 350) //VGA
    {
        fontSize = 18;
        logoSize = 30;
        tableFontSize = 14;
        colSize = 22;
        if (size.width() >= 1180 && size.height() >= 600) //HD
        {
            fontSize = 24;
            logoSize = 50;
            tableFontSize = 20;
            colSize = 28;
            if (size.width() >= 1820 && size.height() >= 950)  //FullHD
            {
                fontSize = 40;
                logoSize = 70;
                tableFontSize = 30;
                colSize = 35;
            }
        }
    }
    //SportRoomUtils::getWidgetMaximumFontSize(ui->teamANameLabel, ui->teamANameLabel->text());
    f = ui->teamANameLabel->font();
    f.setPointSizeF(fontSize);

    ui->teamANameLabel->setFont(f);
    ui->teamBNameLabel->setFont(f);
    ui->teamAPointsLabel->setFont(f);
    ui->teamBPointsLabel->setFont(f);

    f = ui->matchResultTableView->font();
    f.setPointSizeF(tableFontSize);
    ui->matchResultTableView->setFont(f);
    ui->dayLabel->setFont(f);

    for (int i = 0; i< 7 ; i++)
    {
        ui->matchResultTableView->setRowHeight(i,tableFontSize * 2);
    }
    //ui->matchResultTableView->setHorizontalHeader()
    QHeaderView *header = ui->matchResultTableView->horizontalHeader();
    header->setFont(f);

    ui->teamALogoLabel->setMinimumSize(QSize(logoSize, logoSize));
    ui->teamALogoLabel->setMaximumSize(QSize(logoSize, logoSize));
    ui->teamBLogoLabel->setMinimumSize(QSize(logoSize, logoSize));
    ui->teamBLogoLabel->setMaximumSize(QSize(logoSize, logoSize));

    ui->matchResultTableView->setColumnWidth(0, colSize);
    ui->matchResultTableView->setColumnWidth(2, colSize);

    for (int i = 4; i <=10 ; i++){
       ui->matchResultTableView->setColumnWidth(i, colSize * 3);
    }

    drawTeamLogoImages();
    drawTopImages(((size.width() -12*2-5) / 6) * 0.8, topHeight * 0.9);
    drawBottomImages(((size.width() -12*2-5) / 6) * 0.8, bottomHeight * 0.9);

}
