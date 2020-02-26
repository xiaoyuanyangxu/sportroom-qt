#include "selectplayerdialog.h"
#include "ui_selectplayerdialog.h"

#include <QCompleter>

SelectPlayerDialog::SelectPlayerDialog(PlayerDatamodel *model, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SelectPlayerDialog)
{
    ui->setupUi(this);

    QStringList l;
    model->getPlayerNameList(l);

    QCompleter *completer = new QCompleter(l, this);

    completer->setCaseSensitivity(Qt::CaseInsensitive);

    ui->lineEdit->setCompleter(completer);
}


SelectPlayerDialog::~SelectPlayerDialog()
{
    delete ui;
}

QString SelectPlayerDialog::getPlayerName()
{
    return ui->lineEdit->text();
}
