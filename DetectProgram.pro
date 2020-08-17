#-------------------------------------------------
#
# Project created by QtCreator 2019-12-31T14:36:58
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DetectProgram_UseYolov3_WithXML
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

SOURCES += \
        Authorization2.cpp \
        detector.cpp \
        improcess.cpp \
        jj_obi_detect.cpp \
        main.cpp \
        mainwindow.cpp \
        tinyxml/tinystr.cpp \
        tinyxml/tinyxml.cpp \
        tinyxml/tinyxmlerror.cpp \
        tinyxml/tinyxmlparser.cpp

HEADERS += \
        Authorization2.h \
        darknet.h \
        detector.h \
        improcess.h \
        jj_obj_detect.h \
        mainwindow.h \
        tinyxml/tinystr.h \
        tinyxml/tinyxml.h

FORMS += \
        mainwindow.ui

INCLUDEPATH += /usr/local/include/opencv4
INCLUDEPATH += /usr/local/include/opencv4/opencv2
LIBS += /usr/local/lib/libopencv_*
LIBS += $$OUT_PWD/../res/libdarknet.so

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
