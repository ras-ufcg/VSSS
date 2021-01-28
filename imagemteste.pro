#-------------------------------------------------
#
# Project created by QtCreator 2019-06-13T10:22:07
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = opencvtest
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

#INCLUDEPATH += C:\opencv\build\include

#LIBS += C:\opencv-build\bin\libopencv_core320.dll
#LIBS += C:\opencv-build\bin\libopencv_highgui320.dll
#LIBS += C:\opencv-build\bin\libopencv_imgcodecs320.dll
#LIBS += C:\opencv-build\bin\libopencv_imgproc320.dll
#LIBS += C:\opencv-build\bin\libopencv_features2d320.dll
#LIBS += C:\opencv-build\bin\libopencv_calib3d320.dll


INCLUDEPATH +=C:\opencv\build\include # C:\OpenCV3\opencv\opencv_bin2\install\include

LIBS += -L"C:\opencv-build\install\x86\mingw\bin"
LIBS += -llibopencv_core320 -llibopencv_highgui320 -llibopencv_imgproc320 -llibopencv_features2d320 -llibopencv_calib3d320 -llibopencv_imgcodecs320 -llibopencv_video320 -llibopencv_videoio320 -llibopencv_videostab320

# more correct variant, how set includepath and libs for mingw
# add system variable: OPENCV_SDK_DIR=D:/opencv/opencv-build/install
# read http://doc.qt.io/qt-5/qmake-variable-reference.html#libs

#INCLUDEPATH += $$(OPENCV_SDK_DIR)/include

#LIBS += -L$$(OPENCV_SDK_DIR)/x86/mingw/lib \
#        -lopencv_core320        \
#        -lopencv_highgui320     \
#        -lopencv_imgcodecs320   \
#        -lopencv_imgproc320     \
#        -lopencv_features2d320  \
#        -lopencv_calib3d320

RESOURCES +=
