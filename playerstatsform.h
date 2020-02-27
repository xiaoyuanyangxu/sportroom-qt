#ifndef PLAYERSTATSFORM_H
#define PLAYERSTATSFORM_H

#include "matchstatus.h"
#include "playerdatamodel.h"

#include <QWidget>

namespace Ui {
class PlayerStatsForm;
}

class PlayerStatsForm : public QWidget
{
    Q_OBJECT

public:
    explicit PlayerStatsForm(QWidget *parent = nullptr);
    ~PlayerStatsForm();

    void setModels(MatchStatus * status, PlayerDatamodel * players);

public slots:
    void statusContentChanged();
    void playerContentChanged();

private:
    Ui::PlayerStatsForm *ui;

    MatchStatus * status;
    PlayerDatamodel * players;

    QString playerAName;
    QString playerBName;
};

#endif // PLAYERSTATSFORM_H
