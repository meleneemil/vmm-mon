#include "qsocketclient.h"


QSocketClient::QSocketClient()
{
    socket = new QLocalSocket();

    connect(socket, SIGNAL(readyRead()), this, SLOT(readSocket()));
    connect(socket, SIGNAL(error(QLocalSocket::LocalSocketError)),
            this, SLOT(displayError(QLocalSocket::LocalSocketError)));

    socket_requests_timer = new QTimer();
    connect(socket_requests_timer, SIGNAL(timeout()), this, SLOT(connectToServer()));


    startRequests();
}

void QSocketClient::readSocket()
{
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

void QSocketClient::connectToServer()
{
    blockSize = 0;
    socket->abort();
    socket->connectToServer("vmm-mon-server");
}

void QSocketClient::startRequests()
{
    socket_requests_timer->start(1);
}

void QSocketClient::stopRequests()
{
    socket_requests_timer->stop();
}

void QSocketClient::displayError(QLocalSocket::LocalSocketError socketError)
{
    switch (socketError) {
    case QLocalSocket::ServerNotFoundError:
        qDebug() << "The host was not found. Please check the host name and port settings.";
        break;
    case QLocalSocket::ConnectionRefusedError:
        qDebug() << "The connection was refused by the peer. Make sure the fortune server is running, and check that the host name and port settings are correct.";
        break;
    case QLocalSocket::PeerClosedError:
        break;
    default:
        qDebug() << "The following error occurred: %1." <<  (socket->errorString());
    }
}
