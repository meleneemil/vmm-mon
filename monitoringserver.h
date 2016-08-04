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
    QTimer *timer1;
    QTimer *timer2;
    QTimer *timer3;
    QTimer *timer4;
    QTimer *update_timer;
    QTimer *rate_timer;
    std::vector<Chamber*> chambers;
    std::vector<Chip*> chips;
    QMainCanvas *c_chipStatistics;
    int noOfChambers=0;
    int noOfChips=0;
    int canvas_size_in_x;
    int canvas_size_in_y;
    int fill_counter;
    int fill_counter_1sec_ago;
    int rate_interval = 10000;
    QSocketClient *client;

    void configure();
    void makeCanvases();
    void calculate_canvas_dimensions();
private slots:
    void FillTest1();
    void FillTest2();
    void FillTest3();
    void FillTest4();
    void UpdatePads();
    void printRate();

};

#endif // MONITORINGSERVER_H
