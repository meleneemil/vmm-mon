#ifndef MAINWINDOW_H
#define MAINWINDOW_H
//C includes
#include <vector>
#include <iostream>
//QT includes
#include <QWidget>
#include <QTimer>
#include <QMainWindow>
#include <QFile>
//"local" includes
#include "chamber.h"
#include "chip.h"
#include "qsocketclient.h"

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
    void createCanvas();
    void setupCanvas();
    void insertItem(QString);
    void drawAllChips();
    void drawSelectedItems();
    Chamber* findChamber(QString);
    Chip* findChip(QString);
private slots:
    void startCanvasUpdates();
    void stopCanvasUpdates();
    void UpdatePads();
    void start_random_fill();
    void FillTest();
    void treeSelectionChanged();

    void on_b_clearTreeSelection_released();

private:
    Ui::MainWindow *ui;

    QTimer *timer;
    QTimer *update_timer;
    std::vector<Chamber*> chambers;
    std::vector<Chip*> chips;
    QMainCanvas *c_main;
    QMainCanvas *c_event;
    QSocketClient *client;
    int noOfChambers=0;
    int noOfChips=0;
    int canvas_size_in_x;
    int canvas_size_in_y;
    int fill_counter;
};

#endif // MAINWINDOW_H
