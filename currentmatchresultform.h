#ifndef CURRENTMATCHRESULTFORM_H
#define CURRENTMATCHRESULTFORM_H

#include <QWidget>
#include "matchstatus.h"
#include "statedatamodel.h"

namespace Ui {
class CurrentMatchResultForm;
}

class CurrentMatchResultForm : public QWidget
{
    Q_OBJECT
public:
    explicit CurrentMatchResultForm(QWidget *parent = nullptr);
    ~CurrentMatchResultForm();

    void setModels(MatchStatus * matchModel, StateDatamodel * stateModel);

public slots:
    void contentChanged();

private:
    void initializeResultTable();

private:
    Ui::CurrentMatchResultForm *ui;

    MatchStatus * matchModel;
    StateDatamodel * stateModel;
};

#endif // CURRENTMATCHRESULTFORM_H
