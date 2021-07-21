#include "currentmatchresultdialog.h"
#include "ui_currentmatchresultdialog.h"

CurrentMatchResultDialog::CurrentMatchResultDialog(MatchStatus* statusModel, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CurrentMatchResultDialog)
{
    ui->setupUi(this);
    ui->widget->setStatusModel(statusModel);
    this->statusModel = statusModel;
    QObject::connect(statusModel, &MatchStatus::contentChanged,
                     this, &CurrentMatchResultDialog::contentChanged);
    contentChanged();
}

CurrentMatchResultDialog::~CurrentMatchResultDialog()
{
    delete ui;
}

void CurrentMatchResultDialog::contentChanged()
{
    int currentMath, currentGame;
    QString playerAName, playerBName;
    bool visible = statusModel->getElementState(0x02);
    if (visible) {
        statusModel->getCurrentMatch(currentMath, currentGame);
        statusModel->getPlayerName(currentMath, playerAName, playerBName);
        if (playerAName.length() < 2 && playerBName.length() < 2) {
            visible = false;
        }
    }
    ui->widget->setVisible(visible);
}

