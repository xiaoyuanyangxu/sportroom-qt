#ifndef TEAMRESULTFORM_H
#define TEAMRESULTFORM_H

#include <QLabel>
#include <QWidget>
#include "matchstatus.h"
#include "statedatamodel.h"

namespace Ui {
class TeamResultForm;
}

class TeamResultForm : public QWidget
{
    Q_OBJECT

public:
    explicit TeamResultForm(QWidget *parent = nullptr);
    ~TeamResultForm();

    void setStatusModel(MatchStatus * statusModel, StateDatamodel *stateModel);

public slots:
    void contentChanged();

private:
    Ui::TeamResultForm *ui;

    int nameColWidth;
    int totalWidth;

    MatchStatus * statusModel;
    StateDatamodel * stateModel;
};

#endif // TEAMRESULTFORM_H
