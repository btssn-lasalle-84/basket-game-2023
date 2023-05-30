#-------------------------------------------------
#
# Project created by QtCreator 2023-03-23T13:47:53
#
#-------------------------------------------------

QT       += core gui widgets
QT       += bluetooth

TARGET = BasketGameUI
TEMPLATE = app


SOURCES += main.cpp\
        basketgame.cpp \
        communication.cpp \
        puissance4.cpp \
        equipe.cpp \
    seance.cpp

HEADERS  += \
    basketgame.h \
    communication.h \
    puissance4.h \
    equipe.h \
    seance.h

FORMS    += \
    basketgame.ui

CONFIG(release, debug|release):DEFINES+=QT_NO_DEBUG_OUTPUT

RESOURCES += \
    ressources.qrc
