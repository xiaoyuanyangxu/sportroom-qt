#include "teamresultdialog.h"
#include "ui_teamresultdialog.h"

TeamResultDialog::TeamResultDialog(MatchStatus* matchModel,
                                   StateDatamodel * stateModel,
                                   QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TeamResultDialog)
{
    ui->setupUi(this);

    this->matchModel = matchModel;
    this->stateModel = stateModel;

    setAttribute(Qt::WA_QuitOnClose, false);

    ui->widget->setStatusModel(matchModel, stateModel);

    QObject::connect(matchModel, &MatchStatus::contentChanged,
                     this, &TeamResultDialog::contentChanged);
    QObject::connect(stateModel, &StateDatamodel::contentChanged,
                     this, &TeamResultDialog::contentChanged);

    contentChanged();
}

TeamResultDialog::~TeamResultDialog()
{
    delete ui;
}

void TeamResultDialog::contentChanged()
{
    ui->widget->setVisible(stateModel->getElementState(0x01));
}
