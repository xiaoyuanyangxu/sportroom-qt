#ifndef SLIDINGSTACKEDWIDGET_H
#define SLIDINGSTACKEDWIDGET_H
#include <QStackedWidget>
#include <QEasingCurve>
#include <QParallelAnimationGroup>

class SlidingStackedWidget : public QStackedWidget {

       Q_OBJECT
public:
       //! Animation direction
       enum t_direction {
               LEFT2RIGHT,
               RIGHT2LEFT,
               TOP2BOTTOM,
               BOTTOM2TOP,
               AUTOMATIC
       };
       SlidingStackedWidget(QWidget *parent);

public slots:
       //! Set animation speed
       void setSpeed(int speed);
       //! Set easing curve
       void setAnimation(enum QEasingCurve::Type animationtype);
       //! Change positioning mode
       void setVerticalMode(bool vertical = true);
       //! Enables page wrap for \c slideInNext and \c slideInPrev
       void setWrap(bool wrap);
       //! Slide to next page
       bool slideInNext();
       //! Slide to previous page
       bool slideInPrev();
       //! Slide to page x
       void slideInIdx(int idx, enum t_direction direction=AUTOMATIC);
        //! Slide to page with widget
       void slideInWgt(QWidget * widget, enum t_direction direction=AUTOMATIC);
signals:
       //! Animation is finished
       void animationFinished(void);

protected slots:
       void animationDoneSlot(void);

protected:
       QWidget *m_mainwindow;
       int m_speed;
       enum QEasingCurve::Type m_animationtype;
       bool m_vertical;
       int m_now;
       int m_next;
       bool m_wrap;
       QPoint m_pnow;
       bool m_active;
       QList<QWidget*> blockedPageList;
       QParallelAnimationGroup *animgroup;
};
#endif // SLIDINGSTACKEDWIDGET_H
