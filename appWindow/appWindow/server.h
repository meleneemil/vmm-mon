#ifndef SERVER_H
#define SERVER_H

#include <QDialog>

class QLocalServer;

class Server : public QDialog
{
    Q_OBJECT

public:
    Server(QString);

public slots:
    void sendFortune();

private:
    QLocalServer *server;
    int index;

    QString qstr_chip;
    QString qstr_strip;
    QString qstr_pdo  ;
    QString qstr_tdo  ;
    QString qstr_bcid ;
    QString msg;

    void randomizeChipName();
    void addSimpleMessageToMsg();
};

#endif
