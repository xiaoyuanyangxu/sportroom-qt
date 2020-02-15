#include "currentmatchresultform.h"
#include "ui_currentmatchresultform.h"

CurrentMatchResultForm::CurrentMatchResultForm(QWidget *parent) :
    statusModel(0),
    QWidget(parent),
    ui(new Ui::CurrentMatchResultForm)
{
    ui->setupUi(this);
    int nameSize = this->size().width()/2 - 5;
    int totalWidth = this->size().width();
    int totalHeight = this->size().height();
    ui->tableWidget->setColumnWidth(0, nameSize);
    for (int i=1 ; i<=5 ; i++)
    {
        ui->tableWidget->setColumnWidth(i, (totalWidth - nameSize)/5);
    }
    ui->tableWidget->setRowHeight(0, totalHeight/2);
    ui->tableWidget->setRowHeight(1, totalHeight/2);
}

CurrentMatchResultForm::~CurrentMatchResultForm()
{
    delete ui;
}

void CurrentMatchResultForm::setStatusModel(MatchStatus *statusModel)
{
    this->statusModel = statusModel;

    QObject::connect(statusModel, &MatchStatus::contentChanged,
                     this, &CurrentMatchResultForm::contentChanged);
    contentChanged();
}

void CurrentMatchResultForm::contentChanged()
{
    QTableWidgetItem * item;
    int game, match;
    QColor color1(0,63,114);
    QColor color2(218,229,237);

    QString playerAName, playerBName;
    statusModel->getCurrentMatch(game, match);
    statusModel->getPlayerName(game, playerAName, playerBName);

    item = new QTableWidgetItem("  " + playerAName + "");
    item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    item->setTextColor(color1);
    ui->tableWidget->setItem(0,0, item);
    item = new QTableWidgetItem("  "  + playerBName + "");
    item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    ui->tableWidget->setItem(1,0, item);
    item->setTextColor(color1);

    for (int i = 0; i<5 ; i++)
    {
        int playerA, playerB;
        statusModel->getPoints(game, i, playerA, playerB);
        if (playerA != 0 || playerB != 0)
        {
            item = new QTableWidgetItem(QString::number(playerA));
            item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            item->setTextColor((i%2==0)?color2:color1);
            ui->tableWidget->setItem(0, i+1, item);

            item = new QTableWidgetItem(QString::number(playerB));
            item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            item->setTextColor((i%2==0)?color2:color1);
            ui->tableWidget->setItem(1, i+1, item);
        }else{

            ui->tableWidget->setItem(0, i+1, new QTableWidgetItem(""));
            ui->tableWidget->setItem(1, i+1, new QTableWidgetItem(""));
        }
    }
}
