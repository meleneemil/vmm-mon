#ifndef QSOCKETCLIENT_H
#define QSOCKETCLIENT_H

#include <QLocalSocket>
#include <QTimer>
#include <QDataStream>

class QSocketClient : public QObject
{
//    Q_OBJECT

public:
    QSocketClient();


private:
    QTimer* socket_requests_timer;
    QLocalSocket *socket;
    quint16 blockSize;
    QString currentRead;

private slots:
    void connectToServer();
    void startRequests();
    void stopRequests();
    void displayError(QLocalSocket::LocalSocketError socketError);
    void readSocket();

};

#endif // QSOCKETCLIENT_H
