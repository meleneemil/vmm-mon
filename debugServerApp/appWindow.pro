#-------------------------------------------------
#
# Project created by QtCreator 2016-06-07T21:05:15
#
#-------------------------------------------------

QT       += core gui
QT           += network widgets

CONFIG +=c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = appWindow
TEMPLATE = app
#LIBS += -L/home/ak/boost_1_60_0/stage/lib -lboost_thread -lboost_filesystem -lboost_system -lrt


#boostinclude="/home/ak/boost_1_60_0/"
#INCLUDEPATH+=$$boostinclude

INCLUDEPATH += $(ROOTSYS)/include
LIBS += -L$(ROOTSYS)/lib -lCore -lCint -lRIO -lNet \
        -lHist -lGraf -lGraf3d -lGpad -lTree \
        -lRint -lPostscript -lMatrix -lPhysics \
        -lGui -lMathCore \
        -Llib -lRIO -lNet -lHist -lTree -lMatrix -lProof -lThread -lCore -lCint -lMathCore  -lTree -lm -ldl -lrt  -pthread   -rdynamic -g

SOURCES += main.cpp\
    server.cpp

HEADERS  += \
    server.h

FORMS    += mainwindow.ui
