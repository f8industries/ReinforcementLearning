#-------------------------------------------------
#
# Project created by QtCreator 2015-11-07T22:07:19
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ReinforcementLearning
TEMPLATE = app



SOURCES += main.cpp\
        dialog.cpp \
    agent.cpp \
    nav.cpp

HEADERS  += dialog.h \
    agent.h \
    nav.h \
    Helpers.h

FORMS    += dialog.ui
