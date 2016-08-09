#ifndef SERVER_H
#define SERVER_H

#include <QUdpSocket>
#include <QTimer>
#include <QUdpSocket>
#include <QByteArray>
#include <QDialog>

class QLocalServer;

class Server : public QDialog
{
    Q_OBJECT

public:
    Server();

public slots:
    void sendData();

private:
    QTimer *timer;
    QUdpSocket *m_socket_sender;
    int index;

    QString qstr_chip;
    QString qstr_strip;
    QString qstr_pdo  ;
    QString qstr_tdo  ;
    QString qstr_bcid ;
    QString msg;

    void randomizeChipName();
};

#endif
