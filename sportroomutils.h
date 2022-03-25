#ifndef UTILS_H
#define UTILS_H

#include <QGraphicsOpacityEffect>
#include <QLabel>
#include <QPointer>
#include <QPropertyAnimation>
#include <QTimer>
#include <QWidget>

class SportRoomUtils
{
public:
    SportRoomUtils();

public:
    static float getWidgetMaximumFontSize(QWidget *widget, QString text);
    static bool drawImage(QLabel *label, QString uri, int w=0, int h=0);
    static QString toCamelCase(const QString& s);
    static void recoverSize(QDialog * dialog, QString id);
    static void storeSize(QDialog * dialog, QString id);
};

class Delay: public QObject {
public:
    Delay(int duration) {
        QTimer::singleShot(duration, this, SLOT(deleteLater()));
    }
};

class Fader : public QObject {
public:
    Fader(QWidget* target, double start, double end, int fade) {
        auto effect = new QGraphicsOpacityEffect(this);
        target->setGraphicsEffect(effect);

        auto anim = new QPropertyAnimation(effect, "opacity", this);
        connect(anim, &QPropertyAnimation::finished, this, &Fader::deleteLater);
        anim->setDuration(fade);
        anim->setStartValue(start);
        anim->setEndValue(end);
        anim->setEasingCurve(QEasingCurve::InCurve);
        anim->start();
    }

    static void fadeInOut(QWidget* target, int fade, int pause) {
        QPointer<QWidget> p(target);
        if(p)connect(new Fader(target, 0.0, 1.0, fade),&Fader::destroyed, [=](){
            if(p)connect(new Delay(pause), &Delay::destroyed, [=](){
                if(p)connect(new Fader(target,1.0,0.0,fade),&Fader::destroyed,[=](){
                    if(p)target->setVisible(false);
                });
            });
        });
    }

    static void fadeOutIn(QWidget* target, int fade, int pause) {
        QPointer<QWidget> p(target);
        if(p)connect(new Fader(target, 1.0, 0.0, fade),&Fader::destroyed, [=](){
            if(p)connect(new Delay(pause), &Delay::destroyed, [=](){
                if(p)connect(new Fader(target,0.0,1.0,fade),&Fader::destroyed,[=](){
                    if(p)target->setVisible(true);
                });
            });
        });
    }
};

#endif // UTILS_H
