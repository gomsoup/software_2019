TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp \
        v4l_wrapper.cpp

HEADERS += \
    v4l_wrapper.h


INCLUDEPATH += /home/gomsoup/kernel/achroimx_kernel/include
INCLUDEPATH += /opt/toolchains/arm-2014.05/opencv/include
LIBS += -L/opt/toolchains/arm-2014.05/opencv/lib


QMAKE_LFLAGS += -lopencv_shape -lopencv_stitching -lopencv_superres -lopencv_videostab -lopencv_calib3d -lopencv_features2d -lopencv_video -lopencv_photo -lopencv_flann -lopencv_face -lopencv_objdetect -lopencv_highgui -lopencv_videoio -lopencv_imgcodecs -lopencv_ml -lopencv_imgproc -lopencv_core
QMAKE_LFLAGS += -DLINUX -DUSE_SOC_MX6 -Wall -O2 -fpermissive -mfloat-abi=softfp -DEGL_API_FB -DGPU_TYPE_VIV -DGL_GLEXT_PROTOTYPES

QMAKE_CXXFLAGS += -std=c++11
