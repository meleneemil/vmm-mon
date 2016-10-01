#include <QtWidgets>
#include <QtNetwork>

#include <stdlib.h>

#include "server.h"

Server::Server():
m_socket_sender(0)
{
    index = 0;

    m_socket_sender = new QUdpSocket();
    bool bind = m_socket_sender->bind(QHostAddress::LocalHost, 2223);
    if(!bind) {
        qDebug() << "unable to bind socket SENDER";
        m_socket_sender->close();
        m_socket_sender->disconnectFromHost();
    }

    //send dummy config

    QByteArray data;
    QString msg;
    msg = "config start";
    data=msg.toStdString().c_str();
    m_socket_sender->writeDatagram(data, QHostAddress::LocalHost, 2224);
    msg = "config MMFE1 VMM1 VMM2";
    data=msg.toStdString().c_str();
    m_socket_sender->writeDatagram(data, QHostAddress::LocalHost, 2224);
    msg = "config MMFE2 VMM3 VMM4";
    data=msg.toStdString().c_str();
    m_socket_sender->writeDatagram(data, QHostAddress::LocalHost, 2224);
    msg = "config MMFE3 VMM5 VMM6";
    data=msg.toStdString().c_str();
    m_socket_sender->writeDatagram(data, QHostAddress::LocalHost, 2224);
    msg = "config end";
    data=msg.toStdString().c_str();
    m_socket_sender->writeDatagram(data, QHostAddress::LocalHost, 2224);


    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(sendData()));
    timer->start(1);
}

void Server::sendData()
{
    index++;
    randomizeChipName();

    QByteArray data;

    for(int i=0;i<100;i++)
    {
        randomizeChipName();
        qstr_trig_cnt = QString::number(index);
        qstr_strip    = QString::number(rand()%64);
        qstr_pdo      = QString::number(rand()%200/2 + rand()%300);
        qstr_tdo      = QString::number(rand()%500/2 + rand()%400/2);
        qstr_bcid     = QString::number(rand()%4096);
        //int n =0                 1             2              3            4            5
        msg =    qstr_trig_cnt+" "+qstr_chip+" "+qstr_strip+" "+qstr_pdo+" "+qstr_tdo+" "+qstr_bcid;

        data=msg.toStdString().c_str();
        m_socket_sender->writeDatagram(data, QHostAddress::LocalHost, 2224);
    }

}

void Server::randomizeChipName()
{

    switch (
            //0
            rand()%12
            ) {
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
