#include "currentmatchresultdialog.h"
#include "statedatamodel.h"
#include "ui_currentmatchresultdialog.h"

CurrentMatchResultDialog::CurrentMatchResultDialog(MatchStatus* matchModel,
                                                   StateDatamodel* stateModel,
                                                   QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CurrentMatchResultDialog)
{
    ui->setupUi(this);
    ui->widget->setModels(matchModel, stateModel);
    this->matchModel = matchModel;
    this->stateModel = stateModel;

    QObject::connect(matchModel, &MatchStatus::contentChanged,
                     this, &CurrentMatchResultDialog::contentChanged);

    QObject::connect(stateModel, &StateDatamodel::contentChanged,
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
    bool visible = stateModel->getElementState(0x02);
    if (visible) {
        matchModel->getCurrentMatch(currentMath, currentGame);
        matchModel->getPlayerName(currentMath, playerAName, playerBName);
        if (playerAName.length() < 2 && playerBName.length() < 2) {
            visible = false;
        }
    }
    ui->widget->setVisible(visible);
}

