#
# Project created by QtCreator 2019-06-24T12:12:42
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = register
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

INCLUDEPATH += /home/gomsoup/kernel/achroimx_kernel/include
INCLUDEPATH += /opt/toolchains/arm-2014.05/opencv/include
LIBS += -L/opt/toolchains/arm-2014.05/opencv/lib
QMAKE_LFLAGS += -lopencv_shape -lopencv_stitching -lopencv_superres -lopencv_videostab -lopencv_calib3d -lopencv_features2d -lopencv_video -lopencv_photo -lopencv_flann -lopencv_face -lopencv_objdetect -lopencv_highgui -lopencv_videoio -lopencv_imgcodecs -lopencv_ml -lopencv_imgproc -lopencv_core
QMAKE_LFLAGS += -DLINUX -DUSE_SOC_MX6 -Wall -O2 -fpermissive -mfloat-abi=softfp -DEGL_API_FB -DGPU_TYPE_VIV -DGL_GLEXT_PROTOTYPES

SOURCES += \
        main.cpp \
        mainwindow.cpp \ \
        v4l_wrapper.cpp

HEADERS += \
        main.h \
        mainwindow.h \ \
        v4l_wrapper.h

FORMS += \
        mainwindow.ui

QMAKE_CXXFLAGS += -std=c++11

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
