#-------------------------------------------------
#
# Project created by QtCreator 2023-03-23T13:47:53
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BasketGameUI
TEMPLATE = app


SOURCES += main.cpp\
        basketgame.cpp

HEADERS  += basketgame.h

FORMS    += basketgame.ui

CONFIG(release, debug|release):DEFINES+=QT_NO_DEBUG_OUTPUT

RESOURCES += \
    ressources.qrc

DISTFILES += \
    ../../../../Downloads/jetonRouge.png \
    ../../../../Downloads/jetonJaune.png \
    ../../../../Downloads/puissance4_7.png
