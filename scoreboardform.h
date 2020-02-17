#ifndef SCOREBOARDFORM_H
#define SCOREBOARDFORM_H

#include "matchstatus.h"

#include <QWidget>

namespace Ui {
class ScoreboardForm;
}

class ScoreboardForm : public QWidget
{
    Q_OBJECT

public:
    explicit ScoreboardForm(QWidget *parent = nullptr);
    ~ScoreboardForm();

    void setStatusModel(MatchStatus * statusModel);

public slots:
    void contentChanged();

private:
    Ui::ScoreboardForm *ui;

    MatchStatus * statusModel;
};

#endif // SCOREBOARDFORM_H
