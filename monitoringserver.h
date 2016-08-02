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
    int noOfChambers=0;
    int noOfChips=0;
    int canvas_size_in_x;
    int canvas_size_in_y;

    void configure();
    void makeCanvases();
    void calculate_canvas_dimensions();


signals:

public slots:
};

#endif // MONITORINGSERVER_H
