#include "playerandteamsform.h"
#include "ui_playerandteamsform.h"

#include <QFileDialog>
#include <QModelIndexList>
#include <QDebug>

PlayerAndTeamsForm::PlayerAndTeamsForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PlayerAndTeamsForm)
{
    ui->setupUi(this);
}

PlayerAndTeamsForm::~PlayerAndTeamsForm()
{
    delete ui;
}

void PlayerAndTeamsForm::setModel(PlayerDatamodel *model)
{
    this->model = model;
    ui->tableView->setModel(model);

    //ui->tableView->verticalHeader()->hide();
    ui->tableView->setShowGrid(true);

    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    for (int i = 2; i <=12 ; i++){
       ui->tableView->setColumnWidth(i, 70);
    }
    QHeaderView *header = ui->tableView->horizontalHeader();
    header->setSectionResizeMode(0, QHeaderView::Stretch);
    header->setSectionResizeMode(1, QHeaderView::Stretch);

    //QHeaderView *verticalHeader = ui->tableView->verticalHeader();
    //verticalHeader->sectionResizeMode(QHeaderView::Fixed);

    ui->tableView->resizeColumnsToContents();

    /*
    QObject::connect(model, &MatchStatus::contentChanged,
                     this, &FullMatchResultForm::contentChanged);
    contentChanged();    */


}

void PlayerAndTeamsForm::on_importPushButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                   "Select CSV File",
                                                   "",
                                                   tr("CSV (*.csv)"));
    if (!fileName.isEmpty())
    {
        model->importPlayerList(fileName);
    }
}

void PlayerAndTeamsForm::on_deletePushButton_clicked()
{
    QModelIndexList indexes = ui->tableView->selectionModel()->selection().indexes();

    if (indexes.size() > 0)
    {
        model->removeRow(indexes[0].row());
        qDebug() << Q_FUNC_INFO << "Deleted";
        model->submit();
    }
}
