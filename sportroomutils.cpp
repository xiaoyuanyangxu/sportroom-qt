#include "sportroomutils.h"

#include <QLabel>
#include <QWidget>
#include <QDebug>
#include <QImageReader>

#define FONT_PRECISION (0.5)

SportRoomUtils::SportRoomUtils()
{

}

QString SportRoomUtils::toCamelCase(const QString& s)
{
    QStringList parts = s.split(' ', QString::SkipEmptyParts);
    for (int i = 0; i < parts.size(); ++i) {
        parts[i] = parts[i].toLower();
        parts[i].replace(0, 1, parts[i][0].toUpper());
    }

    return parts.join(" ");
}

float SportRoomUtils::getWidgetMaximumFontSize(QWidget *widget, QString text)
{
    QFont font = widget->font();
    const QRect widgetRect = widget->contentsRect();
    const float widgetWidth = widgetRect.width() * 0.8;
    const float widgetHeight = widgetRect.height() * 0.8;

    QRectF newFontSizeRect;
    float currentSize = font.pointSizeF();

    float step = currentSize/2.0;

    /* If too small, increase step */
    if (step<=FONT_PRECISION){
        step = FONT_PRECISION*4.0;
    }

    float lastTestedSize = currentSize;

    float currentHeight = 0;
    float currentWidth = 0;
    if (text==""){
        return currentSize;
    }

    /* Only stop when step is small enough and new size is smaller than QWidget */
    while(step>FONT_PRECISION || (currentHeight > widgetHeight) || (currentWidth > widgetWidth)){
        /* Keep last tested value */
        lastTestedSize = currentSize;

        /* Test label with its font */
        font.setPointSizeF(currentSize);
        /* Use font metrics to test */
        QFontMetricsF fm(font);

        /* Check if widget is QLabel */
        QLabel *label = qobject_cast<QLabel*>(widget);
        if (label) {
            newFontSizeRect = fm.boundingRect(widgetRect, (label->wordWrap()?Qt::TextWordWrap:0) | label->alignment(), text);
        }
        else{
            newFontSizeRect = fm.boundingRect(widgetRect,  0, text);
        }

        currentHeight = newFontSizeRect.height();
        currentWidth = newFontSizeRect.width();

        /* If new font size is too big, decrease it */
        if ((currentHeight > widgetHeight) || (currentWidth > widgetWidth)){
            qDebug() << "-- contentsRect()" << label->contentsRect() << "rect"<< label->rect() << " newFontSizeRect" << newFontSizeRect << "Tight" << text << currentSize;
            currentSize -=step;
            /* if step is small enough, keep it constant, so it converge to biggest font size */
            if (step>FONT_PRECISION){
                step/=2.0;
            }
            /* Do not allow negative size */
            if (currentSize<=0){
                break;
            }
        }
        /* If new font size is smaller than maximum possible size, increase it */
        else{
            qDebug() << "++ contentsRect()" << label->contentsRect() << "rect"<< label->rect() << " newFontSizeRect" << newFontSizeRect << "Tight" << text << currentSize;
            currentSize +=step;
        }
    }
    return lastTestedSize;
}



bool SportRoomUtils::drawImage(QLabel *label, QString uri)
{
    if (uri.isEmpty())
    {
        QPixmap pixmap;
        label->setPixmap(pixmap);
        return false;
    }

    int w = label->width()-8;
    int h = label->height()-8;

    QImageReader imageReader(uri);

    imageReader.setScaledSize(label->size());
    if (!imageReader.canRead())
    {
        return false;
    }
    QImage image = imageReader.read();
    QPixmap pixmap = QPixmap::fromImage(image);

    // set a scaled pixmap to a w x h window keeping its aspect ratio
    label->setPixmap(pixmap);

    return true;
}
