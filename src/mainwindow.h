#ifndef MAINWINDOW_H
#define MAINWINDOW_H
//local
#include "chamber.h"
#include "chip.h"
#include "canvas.h"
#include "chip.h"
#include "chamber.h"
//QT includes
#include <QMainWindow>
#include <QUdpSocket>
#include <QWidget>
#include <QStringList>
#include <QTreeWidgetItem>
#include <QDebug>
#include <QDir>
#include <QTimer>
#include <QCloseEvent>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

private:
    void closeEvent(QCloseEvent *bar){exit(0);};
    Ui::MainWindow *ui;

    QUdpSocket *m_socket_receiver;

    QHostAddress fromIP;
    QByteArray incomingDatagram;
    int eventCount;
    int canvas_size_in_y;
    int canvas_size_in_x;
    bool isConfigured;
    bool debugMode;
    std::vector<Chamber*> chambers;
    std::vector<Chip*> chips;
    QMainCanvas* c_main;
    QMainCanvas* c_event;
    int fill_counter;
    int last_trig_cnt;
    QString tmp_list1;
    std::vector<std::vector<QString>> config_table;
    std::vector<QString> config_row;
    QTimer* update_timer;
    QTimer* mainC_update_timer;
    QTimer* eventC_update_timer;
    int packets_recvd=0;

    bool isPaused=false;

private slots:
    void openUdpConnection();
    void createCanvas();
    void setupCanvas();
    void readUdpPacketData();
    /// DATA HANDLING
    void writeDataSimple(QString);
    void setupConfigLists();
    void fill(int,QString,int,int,int,int);
    /// DRAWING FUNCTIONS
    void drawAllChips();
    void drawSelectedItems();
    void resetAllHistos();
    void deleteAllHistos();
    /// CANVAS UPDATE CONTROL --------------------------------------------------------------------------------------------------
    void startCanvasUpdates();
    void stopCanvasUpdates();
    void updatePads();
    void mainC_updatePads();
    void eventC_updatePads();
    /// UI CONTROL ---------------------------------------------------------------------------------------------
    void treeSelectionChanged();
    /// TOOLS --------------------------------------------
    Chamber* findChamber(QString);
    Chip* findChip(QString);
    void printInfo();
    void debug(QString);
    void on_showStatisticsCheckBox_stateChanged(int arg1);
    void on_showEventCheckBox_stateChanged(int arg1);
    void on_b_Reset_released();
    void on_b_Pause_released();
    void on_b_clearTreeSelection_released();
};
#endif // MAINWINDOW_H
