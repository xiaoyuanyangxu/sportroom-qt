#ifndef CURRENTMATCHRESULTFORM_H
#define CURRENTMATCHRESULTFORM_H

#include <QWidget>
#include "matchstatus.h"

namespace Ui {
class CurrentMatchResultForm;
}

class CurrentMatchResultForm : public QWidget
{
    Q_OBJECT

public:
    explicit CurrentMatchResultForm(QWidget *parent = nullptr);
    ~CurrentMatchResultForm();

    void setStatusModel(MatchStatus * statusModel);

public slots:
    void contentChanged();

private:
    void initializeResultTable();

private:
    Ui::CurrentMatchResultForm *ui;

    MatchStatus * statusModel;
};

#endif // CURRENTMATCHRESULTFORM_H
