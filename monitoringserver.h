#ifndef MONITORINGSERVER_H
#define MONITORINGSERVER_H
//C includes
#include <vector>
//QT includes
#include <QWidget>
#include <QTimer>
//ROOT includes
#include <TH1.h>
//"local" includes
#include "canvas.h"
#include "chamber.h"
#include "qsocketclient.h"
#include "chip.h"

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
    QTimer *timer;
    QTimer *timer_update;
    std::vector<QTimer*> timer_vector;
    int fill_counter;
    QSocketClient *client;

    void configure();
    void makeCanvases();
    void calculate_canvas_dimensions();
private slots:
    void FillTest();
    void UpdatePads();
    void UpdatePad(int);
    void startClient();


signals:

public slots:
};

#endif // MONITORINGSERVER_H
