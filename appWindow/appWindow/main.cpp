#include <QApplication>
#include <QProcess>
#include <string>
#include <iostream>

#include <QLocalServer>
#include <QLocalSocket>
//#include <qlocalserver.h>
//#include <qlocalsocket.h>

#include <QtNetwork>

#include "server.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QLocalServer::removeServer("vmm-mon-server");
    Server* server = new Server("vmm-mon-server");
    qDebug()<<"Server running";
    return a.exec();
}
