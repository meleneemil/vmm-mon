#ifndef QSOCKETCLIENT_H
#define QSOCKETCLIENT_H

#include <QLocalSocket>
#include <QLocalServer>
#include <QTimer>
#include <QDataStream>

#include "datahandler.h"
#include "chamber.h"
#include "canvas.h"

class QSocketClient : public QObject
{
    Q_OBJECT

public:
    QSocketClient(int,
                  std::vector<Chamber*>,
                  std::vector<Chip*>,
                  QMainCanvas*
                  );


private:
    QTimer* socket_requests_timer;
    QLocalSocket *socket;
    quint16 blockSize;
    QString currentRead;
    int request_timeout;
    DataHandler *handler;
    int noOfSuccessfulRequests;

private slots:
    void connectToServer();
    void startRequests();
    void stopRequests();
    void displayError(QLocalSocket::LocalSocketError socketError);
    void readSocket();
    void sendDataToHandler(QString);

};

#endif // QSOCKETCLIENT_H
