#ifndef QSOCKETCLIENT_H
#define QSOCKETCLIENT_H

#include <QLocalSocket>
#include <QTimer>

class QSocketClient
{
public:
    QSocketClient();


private:
    QTimer* socket_requests_timer;
    QLocalSocket *socket;
    quint16 blockSize;
};

#endif // QSOCKETCLIENT_H
