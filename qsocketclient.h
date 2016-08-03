#ifndef QSOCKETCLIENT_H
#define QSOCKETCLIENT_H

#include <QLocalSocket>
#include <QTimer>
#include <QDataStream>

class QSocketClient : public QObject
{
    Q_OBJECT

public:
    QSocketClient(int);


private:
    QTimer* socket_requests_timer;
    QLocalSocket *socket;
    quint16 blockSize;
    QString currentRead;
    int request_timeout;

private slots:
    void connectToServer();
    void startRequests();
    void stopRequests();
    void displayError(QLocalSocket::LocalSocketError socketError);
    void readSocket();
    void sendDataToHandler();

};

#endif // QSOCKETCLIENT_H
