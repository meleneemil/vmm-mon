#include "qsocketclient.h"

QSocketClient::QSocketClient(int new_request_timeout)
{


    socket = new QLocalSocket();

//handler = new Data

    //when socket is ready to read, call readSocket()
    connect(socket, SIGNAL(readyRead()), this, SLOT(readSocket()));
    //when connection fails, print error (and request_timetout changes to >1sec)
    connect(socket, SIGNAL(error(QLocalSocket::LocalSocketError)),
            this, SLOT(displayError(QLocalSocket::LocalSocketError)));

    socket_requests_timer = new QTimer();
    //Every *request_timeout*, call connectToServer()
    connect(socket_requests_timer, SIGNAL(timeout()), this, SLOT(connectToServer()));

    //Start with request_timeout of 1ms
    //if socket fails, then we stop.
    //USER must restart
    request_timeout = new_request_timeout;

    startRequests();
}

void QSocketClient::connectToServer()
{
    blockSize = 0;
    socket->abort();
    socket->connectToServer("vmm-mon-server");
//    qDebug() << "CONNECTED";
}

void QSocketClient::readSocket()
{
    //this code is directly from the Qt example = Fortune Client
        QDataStream in(socket);
        in.setVersion(QDataStream::Qt_4_0);

        if (blockSize == 0) {
            if (socket->bytesAvailable() < (int)sizeof(quint16))
            {
                qDebug() << "no bytes available";
                return;
            }
            in >> blockSize;
        }

        if (in.atEnd())
            return;

        QString nextRead;
        in >> nextRead;

        if (nextRead == currentRead) {
            QTimer::singleShot(0, this, SLOT(connectToServer()));
            return;
        }

    qDebug() << nextRead;
    //    std::vector<QString> eventVector;
    //IMPORTANT TODO: edit to accomodate for multi hit events...
    //this now assumes that each packet tha comes is an event.
    //    eventVector.push_back(nextFortune);
    //    emit fillHistograms(eventVector,eventCounter);
    //    emit drawHistograms();
}

void QSocketClient::sendDataToHandler()
{

}

void QSocketClient::startRequests()
{
    socket_requests_timer->start(request_timeout);
}

void QSocketClient::stopRequests()
{
    socket_requests_timer->stop();
}

void QSocketClient::displayError(QLocalSocket::LocalSocketError socketError)
{
    //if socket could not connect, and gives error
    //then stop requests

//    stopRequests();

    switch (socketError) {
    case QLocalSocket::ServerNotFoundError:
        qDebug() << "The host was not found. Please check the host name and port settings.";
        break;
    case QLocalSocket::ConnectionRefusedError:
        qDebug() << "The connection was refused by the peer. Make sure the fortune server is running, and check that the host name and port settings are correct.";
        qDebug() << "RESTART MONITORING.";
        break;
    case QLocalSocket::PeerClosedError:
        break;
    default:
        qDebug() << "The following error occurred: %1." <<  (socket->errorString());
    }
}
