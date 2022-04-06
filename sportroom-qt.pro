QT       += core gui websockets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    clockdialog.cpp \
    currentmatchresultdialog.cpp \
    currentmatchresultform.cpp \
    fullmatchresultdialog.cpp \
    fullmatchresultform.cpp \
    main.cpp \
    mainwindow.cpp \
    matchstatus.cpp \
    multifuntionaldialog.cpp \
    playerandteamsdialog.cpp \
    playerandteamsform.cpp \
    playerdatamodel.cpp \
    playerstatsdialog.cpp \
    playerstatsform.cpp \
    reflectorconnector.cpp \
    scoreboardform.cpp \
    selectplayerdialog.cpp \
    settingdialog.cpp \
    slidingstackedwidget.cpp \
    sportroomutils.cpp \
    statedatamodel.cpp \
    statusmarkdialog.cpp \
    statusmarkform.cpp \
    statusmarkitem.cpp \
    teamresultdialog.cpp \
    teamresultform.cpp \
    utils.cpp

HEADERS += \
    clockdialog.h \
    currentmatchresultdialog.h \
    currentmatchresultform.h \
    fullmatchresultdialog.h \
    fullmatchresultform.h \
    mainwindow.h \
    matchstatus.h \
    multifuntionaldialog.h \
    playerandteamsdialog.h \
    playerandteamsform.h \
    playerdatamodel.h \
    playerstatsdialog.h \
    playerstatsform.h \
    reflectorconnector.h \
    scoreboardform.h \
    selectplayerdialog.h \
    settingdialog.h \
    slidingstackedwidget.h \
    sportroomutils.h \
    statedatamodel.h \
    statusmarkdialog.h \
    statusmarkform.h \
    statusmarkitem.h \
    teamresultdialog.h \
    teamresultform.h \
    utils.h

FORMS += \
    clockdialog.ui \
    currentmatchresultdialog.ui \
    currentmatchresultform.ui \
    fullmatchresultdialog.ui \
    fullmatchresultform.ui \
    mainwindow.ui \
    multifuntionaldialog.ui \
    playerandteamsdialog.ui \
    playerandteamsform.ui \
    playerstatsdialog.ui \
    playerstatsform.ui \
    scoreboardform.ui \
    selectplayerdialog.ui \
    settingdialog.ui \
    statusmarkdialog.ui \
    statusmarkform.ui \
    teamresultdialog.ui \
    teamresultform.ui

TRANSLATIONS += \
    sportroom-qt_es_ES.ts \
    sportroom-qt_ca_ES.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    images.qrc
