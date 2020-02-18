#ifndef UTILS_H
#define UTILS_H

#include <QWidget>

class SportRoomUtils
{
public:
    SportRoomUtils();

public:
    static float getWidgetMaximumFontSize(QWidget *widget, QString text);
};

#endif // UTILS_H
