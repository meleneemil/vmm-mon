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
    Server* server = new Server();
    return a.exec();
}
