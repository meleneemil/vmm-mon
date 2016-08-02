#ifndef MONITORINGSERVER_H
#define MONITORINGSERVER_H
#include <vector>
#include <QWidget>
#include "canvas.h"
#include "chamber.h"
#include "chip.h"
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
    std::vector<Chamber*> chambers;
    std::vector<std::vector<QString>> chips;
    QMainCanvas *c_chipStatistics;

    void configure();
    void makeCanvases();


signals:

public slots:
};

#endif // MONITORINGSERVER_H
