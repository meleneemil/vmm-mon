#include "mainwindow.h"
#include <QApplication>
#include <TApplication.h>
#include <TSystem.h>
#include <QDebug>
#include <QThread>

int main(int argc, char *argv[])
{
    TApplication rootapp("mmDaq Online Monitoring", &argc, argv);
    rootapp.SetReturnFromRun(true);

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
//    QThread *thread = new QThread();
//    rootapp.moveToThread(thread);
//    thread->start();
//    gSystem->ProcessEvents();
//    rootapp.Run();
//    gSystem->ProcessEvents();
    return a.exec();
}
