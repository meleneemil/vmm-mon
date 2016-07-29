#ifndef MONITORINGSERVER_H
#define MONITORINGSERVER_H

#include <QWidget>
#include "canvas.h"
#include <TH1.h>

class MonitoringServer : public QWidget
{
    Q_OBJECT
public:
    explicit MonitoringServer(QWidget *parent = 0);

private:
    QTimer* fRootTimer;
    QMainCanvas* testCanvas;
    TH1F* myhist;


signals:

public slots:
};

#endif // MONITORINGSERVER_H
