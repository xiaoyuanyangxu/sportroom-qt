#include "mainwindow.h"

#include <QApplication>
#include <QSettings>
#include <QTranslator>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QCoreApplication::setApplicationName("Sport Room");
    QCoreApplication::setOrganizationName("Sport Room");


    QTranslator translator;
    QSettings settings;
    QString language;

    language = settings.value("selected_language", "").toString();
    if (language.isEmpty())
    {
        language = QLocale::system().name();
    }
    if (translator.load("sportroom-qt_"+language))
    {
        qDebug() << "LANGUAGE " << language <<  "Correctly loaded";
    }else{
        qDebug() << "LANGUAGE " << language <<  "Can't be loaded";
    }
    a.installTranslator(&translator);

    MainWindow w;
    w.show();
    return a.exec();
}
