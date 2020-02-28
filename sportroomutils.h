#ifndef UTILS_H
#define UTILS_H

#include <QLabel>
#include <QWidget>

class SportRoomUtils
{
public:
    SportRoomUtils();

public:
    static float getWidgetMaximumFontSize(QWidget *widget, QString text);
    static void drawImage(QLabel *label, QString uri);
    static QString toCamelCase(const QString& s);
};

#endif // UTILS_H
