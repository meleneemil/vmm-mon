#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include "monitoringserver.h"
#include "chamber.h"
#include "chip.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    void configure();
    void calculate_canvas_dimensions();
    void makeCanvases();
//    void on_b_startMonitoring_released();

private:
    Ui::MainWindow *ui;
    MonitoringServer mon;


    std::vector<Chamber*> chambers;
    std::vector<Chip*> chips;
    QMainCanvas *c_main;
    int noOfChambers=0;
    int noOfChips=0;
    int canvas_size_in_x;
    int canvas_size_in_y;
};

#endif // MAINWINDOW_H
