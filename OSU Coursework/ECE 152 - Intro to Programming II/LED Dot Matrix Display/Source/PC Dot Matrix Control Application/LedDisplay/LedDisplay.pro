#-------------------------------------------------
#
# Project created by QtCreator 2012-06-09T17:31:59
#
#-------------------------------------------------

QT       += core gui

TARGET = LedDisplay
TEMPLATE = app

DEPENDPATH += . qextserial
INCLUDEPATH += . qextserial

include(qextserial/src/qextserialport.pri)

SOURCES += main.cpp\
        leddisplay.cpp \
    varclass.cpp

HEADERS  += leddisplay.h \
    varclass.h

FORMS    += leddisplay.ui

LIBS += -lboost_system \
    -lws2_32 \
    -D _WIN32_WINNT=0x0501

RESOURCES += \
    LedResources.qrc
