#-------------------------------------------------
#
# Project created by QtCreator 2016-07-29T18:51:52
#
#-------------------------------------------------

#Simple monitoring of VMM DAQ.
#Aimilianos Koulouris.

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QMAKE_CXXFLAGS += -std=c++11
TARGET = vmm-mon
TEMPLATE = app

INCLUDEPATH += $(ROOTSYS)/include

LIBS += -L$(ROOTSYS)/lib -lCore -lCint -lRIO -lNet \
        -lHist -lGraf -lGraf3d -lGpad -lTree \
        -lRint -lPostscript -lMatrix -lPhysics \
        -lGui \
        -lRIO -lNet -lHist -lTree -lMatrix -lProof -lThread -lCore -lCint -lMathCore  -lTree -lm -ldl -rdynamic


SOURCES += main.cpp\
        mainwindow.cpp \
    canvas.cxx \
    chamber.cpp \
    chip.cpp

HEADERS  += mainwindow.h \
    canvas.h \
    chamber.h \
    chip.h

FORMS    += mainwindow.ui

RESOURCES += \
    logo.qrc

unix:MOC_DIR = build
unix:OBJECTS_DIR = build
unix:RCC_DIR = build
unix:UI_DIR = build
