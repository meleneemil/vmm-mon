#-------------------------------------------------
#
# Project created by QtCreator 2016-07-29T18:51:52
#
#-------------------------------------------------

#Simple monitoring of VMM DAQ.
#Aimilianos Koulouris.

QT+= core gui network widgets

QMAKE_CXXFLAGS += -std=c++11
TARGET = vmm-mon
TEMPLATE = app

INCLUDEPATH += $(ROOTSYS)/include
INCLUDEPATH += src
#INCLUDEPATH += src/root
INCLUDEPATH += src/struct
INCLUDEPATH += src/rsc

LIBS += -L$(ROOTSYS)/lib \
    -lCore \
    -lHist \
    -lGpad


SOURCES +=\
    src/main.cpp\
    src/mainwindow.cpp\
#    src/root/canvas.cxx\
    src/struct/chamber.cpp\
    src/struct/chip.cpp

HEADERS +=\
    src/mainwindow.h \
#    src/root/canvas.h \
    src/struct/chamber.h \
    src/struct/chip.h

FORMS += src/mainwindow.ui

RESOURCES += \
    src/rsc/logo.qrc

unix:MOC_DIR = build
unix:OBJECTS_DIR = build
unix:RCC_DIR = build
unix:UI_DIR = build

