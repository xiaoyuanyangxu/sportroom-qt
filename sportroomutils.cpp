#include "sportroomutils.h"

#include <QLabel>
#include <QWidget>
#include <QDebug>
#include <QImageReader>
#include <QSettings>
#include <QDialog>
#include <QFile>
#include <QSvgRenderer>
#include <QPixmap>
#include <QPainter>
#include <QIcon>

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

void SportRoomUtils::recoverSize(QDialog *dialog, QString id)
{
    int w,h;

    QSettings settings;

    w = settings.value(id + "_width", "0").toInt();
    h = settings.value(id + "_height", "0").toInt();

    qDebug() << Q_FUNC_INFO << "w: " << w << " h:" <<h << " id: " << id;

    if (w > 0 && h > 0) {
        dialog->resize(w,h);
    }
}

void SportRoomUtils::storeSize(QDialog *dialog, QString id)
{
    qDebug() << Q_FUNC_INFO << dialog->size() << " id: "<< id;

    QSettings settings;

    settings.setValue(id + "_width", dialog->size().width());
    settings.setValue(id + "_height", dialog->size().height());
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



bool SportRoomUtils::drawImage(QLabel *label, QString uri, int w, int h)
{
    qDebug() << Q_FUNC_INFO << uri;
    if (uri.isEmpty())
    {
        QPixmap pixmap;
        label->setPixmap(pixmap);
        return false;
    }

    if (w == 0 || h ==0)
    {
        w = label->width()-8;
        h = label->height()-8;
    }

    QImageReader imageReader(uri);

    imageReader.setScaledSize(label->size());
    if (!imageReader.canRead())
    {
        qDebug() << Q_FUNC_INFO << "Error reading";
        return false;
    }
    QImage image = imageReader.read();
    QPixmap pixmap = QPixmap::fromImage(image);

    // set a scaled pixmap to a w x h window keeping its aspect ratio
    label->setPixmap(pixmap);

    return true;
}


QString SportRoomUtils::contrastColor(const QString& bgColor)
{
    if (bgColor.isNull()) {
        return QColor(Qt::black).name();
    }

    QColor color(bgColor);
    int d = 0;

    // counting the perceptive luminance - human eye favors green color...
    double a = 1 - (0.299 * color.red() + 0.587 * color.green() + 0.114 * color.blue()) / 255;

    if (a < 0.5) {
        d = 4; // bright colors - black font
    } else {
        d = 240; // dark colors - white font
    }
    return  QColor(d, d, d).name();
}

QIcon SportRoomUtils::SvgToQIcon(const QString resource, const QString bgColor, const QString strokeColor)
{
    QFile file(resource);
    file.open(QIODevice::ReadOnly);
    QByteArray baData = file.readAll();
    QString svgData = QString(baData);

    svgData.replace("current", QString("%1").arg(bgColor));
    svgData.replace("stroke_color", QString("%1").arg(strokeColor));

    qDebug() << Q_FUNC_INFO << svgData;

    QSvgRenderer svgRenderer(svgData.toUtf8());
    QPixmap pix(svgRenderer.defaultSize());
    pix.fill(Qt::transparent);
    QPainter pixPainter(&pix);
    svgRenderer.render(&pixPainter);
    QIcon icon(pix);

    return icon;
}
