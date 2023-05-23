#-------------------------------------------------
#
# Project created by QtCreator 2023-03-23T13:47:53
#
#-------------------------------------------------

QT       += core gui
QT       += bluetooth
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BasketGameUI
TEMPLATE = app


SOURCES += main.cpp\
        basketgame.cpp \
        communication.cpp \
    puissance4.cpp \
    equipe.cpp

HEADERS  += \
    basketgame.h \
    communication.h \
    puissance4.h \
    equipe.h

FORMS    += \
    basketgame.ui

CONFIG(release, debug|release):DEFINES+=QT_NO_DEBUG_OUTPUT

RESOURCES += \
    ressources.qrc
