#-------------------------------------------------
#
# Project created by QtCreator 2013-12-07T21:54:25
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = adanto
TEMPLATE = app

SOURCES += main.cpp\
        imagesdisplay.cpp \  
    images_kernel_gl.cpp \
    controlPanel.cpp \
    scannerfiledialog.cpp \
    shared_fuctions.cpp

HEADERS  += imagesdisplay.h \
    images_kernel_gl.h \
    controlPanel.h \
    scannerfiledialog.h \
    inter_class_data.h

OTHER_FILES +=

RESOURCES += \
    shaders.qrc \
    texture_examples.qrc \
    icons.qrc
