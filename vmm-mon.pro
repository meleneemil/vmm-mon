#-------------------------------------------------
#
# Project created by QtCreator 2016-07-29T18:51:52
#
#-------------------------------------------------

#Simple monitoring of VMM DAQ.
#Aimilianos Koulouris.

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = vmm-mon
TEMPLATE = app

INCLUDEPATH += $(ROOTSYS)/include

LIBS += -L$(ROOTSYS)/lib -lCore -lCint -lRIO -lNet \
        -lHist -lGraf -lGraf3d -lGpad -lTree \
        -lRint -lPostscript -lMatrix -lPhysics \
        -lGui \ #-lMathCore \
        -lRIO -lNet -lHist -lTree -lMatrix -lProof -lThread -lCore -lCint -lMathCore  -lTree -lm -ldl  -rdynamic


SOURCES += main.cpp\
        mainwindow.cpp \
    monitoringserver.cpp \
    canvas.cxx \
    qsocketclient.cpp \
    chamber.cpp \
    chip.cpp \
    datahandler.cpp

HEADERS  += mainwindow.h \
    monitoringserver.h \
    canvas.h \
    qsocketclient.h \
    chamber.h \
    chip.h \
    datahandler.h

FORMS    += mainwindow.ui
