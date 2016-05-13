#-------------------------------------------------
#
# Project created by QtCreator 2014-07-11T16:04:05
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = OpenCVTesting
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

INCLUDEPATH += /usr/include/opencv

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
