#include <QtWidgets>
#include <QtNetwork>

#include <stdlib.h>

#include "server.h"
#include <qlocalserver.h>
#include <qlocalsocket.h>

Server::Server(QString serverName)
{
    index = 0;
    server = new QLocalServer(this);
    if (!server->listen(serverName)) {
        QMessageBox::critical(this, tr("Fortune Server"),
                              tr("Unable to start the server: %1.")
                              .arg(server->errorString()));
        close();
        return;
    }

    connect(server, SIGNAL(newConnection()), this, SLOT(sendFortune()));

}

void Server::sendFortune()
{

    index++;
    randomizeChipName();
    //we will add many messages to one string
    {

        randomizeChipName();
        qstr_strip = QString::number(rand()%64);
        qstr_pdo   = QString::number(rand()%200/2 + rand()%300);
        qstr_tdo   = QString::number(rand()%500/2 + rand()%400/2);
        qstr_bcid  = QString::number(rand()%4096);
        //int n =0             1              2            3            4
        msg =qstr_chip+" "+qstr_strip+" "+qstr_pdo+" "+qstr_tdo+" "+qstr_bcid;

        for(int i=0;i<100;i++)
        {
            addSimpleMessageToMsg();
        }
    }//end of additions to msg

    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_0);
    out << (quint16)0;
    out << msg;

    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));

    QLocalSocket *clientConnection = server->nextPendingConnection();
    connect(clientConnection, SIGNAL(disconnected()),
            clientConnection, SLOT(deleteLater()));

    clientConnection->write(block);
    clientConnection->flush();
    clientConnection->disconnectFromServer();
}

void Server::randomizeChipName()
{

    switch (rand()%12) {
    case 0:
        qstr_chip = "Vmm1.0";
        break;
    case 1:
        qstr_chip = "Vmm2.0";
        break;
    case 2:
        qstr_chip = "Vmm3.0";
        break;
    case 3:
        qstr_chip = "Vmm4.0";
        break;
    case 4:
        qstr_chip = "Vmm5.0";
        break;
    case 5:
        qstr_chip = "Vmm6.0";
        break;
    case 6:
        qstr_chip = "Vmm7.0";
        break;
    case 7:
        qstr_chip = "Vmm8.0";
        break;
    case 8:
        qstr_chip = "Vmm9.0";
        break;
    case 9:
        qstr_chip = "Vmm10.0";
        break;
    case 10:
        qstr_chip = "Vmm11.0";
        break;
    case 11:
        qstr_chip = "Vmm12.0";
        break;
    default:
        break;
    }

}

void Server::addSimpleMessageToMsg()
{
    randomizeChipName();
    qstr_strip = QString::number(rand()%64);
    qstr_pdo   = QString::number(rand()%200/2 + rand()%300);
    qstr_tdo   = QString::number(rand()%500/2 + rand()%400/2);
    qstr_bcid  = QString::number(rand()%4096);
    //int n =     0             1              2            3            4
    msg +=" "+qstr_chip+" "+qstr_strip+" "+qstr_pdo+" "+qstr_tdo+" "+qstr_bcid;
}
