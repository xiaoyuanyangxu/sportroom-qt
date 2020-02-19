#ifndef TEAMRESULTFORM_H
#define TEAMRESULTFORM_H

#include <QLabel>
#include <QWidget>
#include "matchstatus.h"

namespace Ui {
class TeamResultForm;
}

class TeamResultForm : public QWidget
{
    Q_OBJECT

public:
    explicit TeamResultForm(QWidget *parent = nullptr);
    ~TeamResultForm();

    void setStatusModel(MatchStatus * statusModel);

public slots:
    void contentChanged();

private:
    Ui::TeamResultForm *ui;

    MatchStatus * statusModel;
};

#endif // TEAMRESULTFORM_H
