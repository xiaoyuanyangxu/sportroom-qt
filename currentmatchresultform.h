#ifndef CURRENTMATCHRESULTFORM_H
#define CURRENTMATCHRESULTFORM_H

#include <QTableWidget>
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

    void setModels(MatchStatus * matchModel, StateDatamodel * stateModel, bool wantSummaryMode);

public slots:
    void contentChanged();

private:
    void initializeResultTable();
    void fullMode(QTableWidgetItem * item, int currentMatch, int currentGame);
    void summaryMode(QTableWidgetItem * item, int currentMatch, int currentGame);

private:
    Ui::CurrentMatchResultForm *ui;

    MatchStatus *    matchModel;
    StateDatamodel * stateModel;

    bool             wantSummaryMode;
};

#endif // CURRENTMATCHRESULTFORM_H
