#ifndef FULLMATCHRESULTFORM_H
#define FULLMATCHRESULTFORM_H

#include "matchstatus.h"

#include <QLabel>
#include <QWidget>

namespace Ui {
class FullMatchResultForm;
}

class FullMatchResultForm : public QWidget
{
    Q_OBJECT

public:
    explicit FullMatchResultForm(QWidget *parent = nullptr);
    ~FullMatchResultForm();

    void setStatusModel(MatchStatus * statusModel);

private:
    void drawImage(QLabel *label, QString uri);

public slots:
    void contentChanged();

private:
    Ui::FullMatchResultForm *ui;

    MatchStatus * statusModel;
};

#endif // FULLMATCHRESULTFORM_H
