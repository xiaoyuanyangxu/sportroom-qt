#ifndef STATUSMARKFORM_H
#define STATUSMARKFORM_H

#include <QWidget>

#include "statedatamodel.h"

namespace Ui {
class StatusMarkForm;
}

class StatusMarkForm : public QWidget
{
    Q_OBJECT

public:
    explicit StatusMarkForm(QWidget *parent = nullptr);
    ~StatusMarkForm();

    void setStatusModel(StateDatamodel *statusModel);

public slots:
    void contentChanged();

private:
    Ui::StatusMarkForm *ui;

    StateDatamodel             *statusModel;
};

#endif // STATUSMARKFORM_H
