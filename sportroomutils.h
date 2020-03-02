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
    static bool drawImage(QLabel *label, QString uri);
    static QString toCamelCase(const QString& s);
    static void recoverSize(QDialog * dialog, QString id);
    static void storeSize(QDialog * dialog, QString id);
};

#endif // UTILS_H
