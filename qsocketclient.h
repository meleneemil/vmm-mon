#ifndef QSOCKETCLIENT_H
#define QSOCKETCLIENT_H

#include <QUdpSocket>

#include "datahandler.h"
#include "chamber.h"
#include "canvas.h"

class QSocketClient : public QObject
{
    Q_OBJECT

public:
    QSocketClient(std::vector<Chamber*>,
                  std::vector<Chip*>,
                  QMainCanvas*
                  );


private:
    QUdpSocket *m_socket_receiver;
    int event_count;

    DataHandler *handler;
    int noOfSuccessfulRequests;

private slots:
    void readData();
    void sendDataToHandler(QString);

};

#endif // QSOCKETCLIENT_H
