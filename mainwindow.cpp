#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //brief description of the mon server:
    //initialized from header file

    /**
      1) Configuration
      2) Create histos
      3) create canvases, and draw
      4) their frames should be ready
      5) show them.
      ...hmmm
      */

}

MainWindow::~MainWindow()
{
    delete ui;
    //    QApplication::quit();
}

//void MainWindow::on_b_startMonitoring_released()
//{
//    new MonitoringServer();
//    this->hide();
//}
