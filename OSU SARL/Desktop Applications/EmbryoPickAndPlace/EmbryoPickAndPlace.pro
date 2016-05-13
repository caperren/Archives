#-------------------------------------------------
#
# Project created by QtCreator 2014-07-13T15:53:01
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = EmbryoPickAndPlace
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    driverboardserial.cpp \
    embryoprocesser.cpp

HEADERS  += mainwindow.h \
    driverboardserial.h \
    embryoprocesser.h

FORMS    += mainwindow.ui

DEPENDPATH += . qextserial

INCLUDEPATH += /usr/include/opencv \
               . qextserial

include(qextserialport-1.2rc/src/qextserialport.pri)

LIBS += -L/usr/lib \
        -lopencv_calib3d \
        -lopencv_contrib \
        -lopencv_core \
        -lopencv_features2d \
        -lopencv_flann \
        -lopencv_gpu \
        -lopencv_highgui \
        -lopencv_imgproc \
        -lopencv_legacy \
        -lopencv_ml \
        -lopencv_objdetect \
        -lopencv_ocl \
        -lopencv_photo \
        -lopencv_stitching \
        -lopencv_superres \
        -lopencv_ts \
        -lopencv_video \
        -lopencv_videostab
