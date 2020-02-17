#include "fullmatchresultform.h"
#include "ui_fullmatchresultform.h"

#include <QDate>
#include <QImageReader>
#include <QDebug>
#include <QContextMenuEvent>
#include <QFileDialog>

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
    ui->matchResultTableView->setColumnWidth(4, 60);
    ui->matchResultTableView->setColumnWidth(5, 60);
    ui->matchResultTableView->setColumnWidth(6, 60);
    ui->matchResultTableView->setColumnWidth(7, 60);
    ui->matchResultTableView->setColumnWidth(8, 60);
    ui->matchResultTableView->setColumnWidth(9, 60);
    ui->matchResultTableView->setColumnWidth(10, 60);
    //ui->matchResultTableView->setRowHeight(5, 60);

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

void FullMatchResultForm::drawImage(QLabel *label, QString uri)
{
    if (uri.isEmpty()) return;

    QImageReader imageReader(uri);
    imageReader.setScaledSize(label->size());
    if (!imageReader.canRead())
    {
        return;
    }
    QImage image = imageReader.read();
    QPixmap pixmap = QPixmap::fromImage(image);

    // set a scaled pixmap to a w x h window keeping its aspect ratio
    label->setPixmap(pixmap);
}

void FullMatchResultForm::createContextMenu()
{
    delete(contextMenu);

    contextMenu  = new QMenu(this);


    QAction *modeAction;
    modeAction   = new QAction(editingMode?QObject::tr("Boardcast Mode"):QObject::tr("Edit Mode"),
                             this);
    connect(modeAction,
            SIGNAL(triggered()),
            this,
            SLOT(switchingMode()));

    contextMenu->addAction(modeAction);
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

    drawImage(ui->teamALogoLabel, statusModel->getTeamALogoFile());
    drawImage(ui->teamBLogoLabel, statusModel->getTeamBLogoFile());

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

    int maxWidth = (this->size().width() - ui->scoreboardWidget->size().width()) / 3;
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

void FullMatchResultForm::on_FullMatchResultForm_customContextMenuRequested(const QPoint &pos)
{


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
    }
}

void FullMatchResultForm::contextMenuEvent(QContextMenuEvent *event)
{
    qDebug(Q_FUNC_INFO, event);
    createContextMenu();
    contextMenu->popup(event->globalPos());
}

void FullMatchResultForm::resizeEvent(QResizeEvent *event)
{
    qDebug() << Q_FUNC_INFO << event->size();
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
