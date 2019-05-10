#-------------------------------------------------
#
# Project created by QtCreator 2016-05-30T18:42:28
# Author: Nauroze Hoath
#
#-------------------------------------------------

QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = objectTracking2
TEMPLATE = app
CONFIG += c++11


SOURCES += main.cpp\
        mainwindow.cpp \
         dialog.cpp
INCLUDEPATH +=C:\opencv\build\include # C:\OpenCV3\opencv\opencv_bin2\install\include

LIBS += -L"C:\opencv-build\install\x86\mingw\bin"
LIBS += -llibopencv_core320 -llibopencv_highgui320 -llibopencv_imgproc320 -llibopencv_features2d320 -llibopencv_calib3d320 -llibopencv_imgcodecs320 -llibopencv_video320 -llibopencv_videoio320 -llibopencv_videostab320

HEADERS  += mainwindow.h \
    dialog.h

FORMS    += mainwindow.ui \
    dialog.ui

