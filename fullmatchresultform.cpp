#include "fullmatchresultform.h"
#include "ui_fullmatchresultform.h"

#include <QDate>
#include <QImageReader>
#include <QDebug>
#include <QContextMenuEvent>
#include <QFileDialog>
#include <QColorDialog>
#include "sportroomutils.h"

FullMatchResultForm::FullMatchResultForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FullMatchResultForm),
    editingMode(false),
    contextMenu(0)
{
    ui->setupUi(this);
    QDate today = QDate::currentDate();
    ui->dayLabel->setText(today.toString("dd/MM/yyyy"));
}

FullMatchResultForm::~FullMatchResultForm()
{
    delete contextMenu;
    delete ui;
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

    QAction *modeAction;
    modeAction   = new QAction(editingMode?QObject::tr("Boardcast Mode"):QObject::tr("Edit Mode"),
                             this);
    connect(modeAction,
            SIGNAL(triggered()),
            this,
            SLOT(switchingMode()));

    contextMenu->addAction(modeAction);

    if (editingMode){
        /*
        QAction *backgroundAction;
        backgroundAction   = new QAction(QObject::tr("Background Color"),
                                 this);
        connect(backgroundAction,
                SIGNAL(triggered()),
                this,
                SLOT(backgroundColor()));

        contextMenu->addAction(backgroundAction);
        */
    }


}

bool FullMatchResultForm::changeImageIcon(QPushButton *pushButton, QString fileName, int maxWidth)
{
    if (!fileName.isEmpty())
    {
        int height = pushButton->size().height();

        QImageReader imageReader(fileName);
        QSize imageSize = imageReader.size();
        int targetWidth = imageSize.width() * ((float)height/imageSize.height());

        if (maxWidth && (targetWidth > maxWidth)) {
            qDebug() << Q_FUNC_INFO << "too big " << targetWidth << " " << maxWidth;
            targetWidth = maxWidth;
            height = imageSize.height() *((float)targetWidth / imageSize.width());
            qDebug() << Q_FUNC_INFO << height;
        }

        imageReader.setScaledSize(QSize(targetWidth, height));
        if (!imageReader.canRead())
        {
            return false;
        }
        QImage image = imageReader.read();
        QPixmap pixmap = QPixmap::fromImage(image);

        // set a scaled pixmap to a w x h window keeping its aspect ratio
        // pushButton->setSizeIncrement(targetWidth, height);
        pushButton->setIconSize(QSize(targetWidth, height));
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

    QDate today = QDate::currentDate();
    ui->dayLabel->setText(today.toString("dd/MM/yyyy"));


    drawAllImages();
}

void FullMatchResultForm::switchingMode()
{
    if (editingMode) {
        editingMode = false;
        ui->imageTopLeftPushButton->setText("");
        ui->imageTopRightPushButton->setText("");
        ui->imageBottomLeftPushButton->setText("");
        ui->imageBottomRightPushButton->setText("");
    }else{
        editingMode = true;
        ui->imageTopLeftPushButton->setText("TopLeft");
        ui->imageTopRightPushButton->setText("TopRight");
        ui->imageBottomLeftPushButton->setText("BottomLeft");
        ui->imageBottomRightPushButton->setText("BottomRight");
        //ui->imageTopLeftPushButton->setStyleSheet("background-color: rgb(255, 255, 255);");
    }
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

void FullMatchResultForm::contextMenuEvent(QContextMenuEvent *event)
{
    qDebug()<< Q_FUNC_INFO<<event;
    createContextMenu();
    contextMenu->popup(event->globalPos());
}


void FullMatchResultForm::on_imageTopLeftPushButton_clicked()
{
    if (!editingMode) {
        return;
    }
    if (setImage("topLeft"))
    {
        ui->imageTopLeftPushButton->setText("");
    }
}

void FullMatchResultForm::on_imageTopRightPushButton_clicked()
{
    if (!editingMode) {
        return;
    }
    if (setImage("topRight"))
    {
        ui->imageTopRightPushButton->setText("");
    }
}

void FullMatchResultForm::on_imageBottomLeftPushButton_clicked()
{
    if (!editingMode) {
        return;
    }
    if (setImage("bottomLeft"))
    {
        ui->imageBottomLeftPushButton->setText("");
    }
}

void FullMatchResultForm::on_imageBottomRightPushButton_clicked()
{
    if (!editingMode) {
        return;
    }
    if (setImage("bottomRight"))
    {
        ui->imageBottomRightPushButton->setText("");
    }
}

void FullMatchResultForm::resizeEvent(QResizeEvent *event)
{
    qDebug() << Q_FUNC_INFO << event->size();

    float fontSize;
    float tableFontSize;
    int logoSize;
    int colSize = 20;
    QFont f;

    fontSize = 16;
    logoSize = 30;
    tableFontSize = 13;

    QSize size = this->size();

    if (size.width() >= 620 && size.height() >= 350) //VGA
    {
        fontSize = 20;
        logoSize = 30;
        tableFontSize = 16;
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

    drawAllImages();
}


void FullMatchResultForm::drawAllImages()
{
    SportRoomUtils::drawImage(ui->teamALogoLabel, statusModel->getTeamALogoFile());
    SportRoomUtils::drawImage(ui->teamBLogoLabel, statusModel->getTeamBLogoFile());

    int maxWidth = (this->size().width() -12*2-5) / 6;
    QString path = statusModel->getImage("bottomRight");
    this->changeImageIcon(ui->imageBottomRightPushButton, path, maxWidth);
    path = statusModel->getImage("bottomLeft");
    this->changeImageIcon(ui->imageBottomLeftPushButton, path, maxWidth);

    maxWidth = this->size().width() / 6;
    path = statusModel->getImage("topLeft");
    this->changeImageIcon(ui->imageTopLeftPushButton, path, maxWidth);
    path = statusModel->getImage("topRight");
    this->changeImageIcon(ui->imageTopRightPushButton, path, maxWidth);
}
