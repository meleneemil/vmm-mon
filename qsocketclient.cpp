#include "qsocketclient.h"

QSocketClient::QSocketClient(
        std::vector<Chamber*> chambers,
        std::vector<Chip*> chips,
        QMainCanvas* c_main
        ):
m_socket_receiver(0),
event_count(0)
{
    noOfSuccessfulRequests=0;

    handler = new DataHandler(chambers,chips,c_main);

    m_socket_receiver = new QUdpSocket(this);
    bool bind = m_socket_receiver->bind(QHostAddress::LocalHost, 2224);
    if(!bind) {
        qDebug() << "unable to bind socket SENDER";
        m_socket_receiver->close();
        m_socket_receiver->disconnectFromHost();
    }

    connect(m_socket_receiver, SIGNAL(readyRead()), this, SLOT(readData()));

}

void QSocketClient::readData(){

    QHostAddress fromIP;
    QByteArray incomingDatagram;

    while(m_socket_receiver->hasPendingDatagrams()) {
        incomingDatagram.resize(m_socket_receiver->pendingDatagramSize());
        m_socket_receiver->readDatagram(incomingDatagram.data(),
                                            incomingDatagram.size(),
                                            &fromIP);

        QString input = QString(incomingDatagram);
        sendDataToHandler(input);
    } // while
    event_count++;
}

void QSocketClient::sendDataToHandler(QString data)
{
    //here we can choose the method.
    //this means, we can choose between the implemented
    //formats and
    //fill strategies

    //the simple one: OLD format, Standard Strategy
        handler->writeDataSimple(data);

    //faster!!! : OLD Format, smart strategy
//    handler->saveDataSendLater(data);
}
