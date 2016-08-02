#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "monitoringserver.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
//    void on_b_startMonitoring_released();

private:
    Ui::MainWindow *ui;
    MonitoringServer mon;
};

#endif // MAINWINDOW_H
