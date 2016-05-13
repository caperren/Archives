#-------------------------------------------------
#
# Project created by QtCreator 2014-07-09T20:57:40
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ThreadedGuiTesting
TEMPLATE = app


SOURCES += main.cpp\
        dialog.cpp \
    countingthread.cpp

HEADERS  += dialog.h \
    countingthread.h

FORMS    += dialog.ui
