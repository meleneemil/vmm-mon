#include "monitoringserver.h"
#include <iostream>
#include <QDebug>
#include <QFile>
#include <QMainWindow>

MonitoringServer::MonitoringServer(QWidget *parent) : QWidget(parent)
{

    //read config file
    //    qDebug() << "[configure]";
    configure();
    //calculate how many chip histos
    //and how many chambers, to Divide the
    //canvas properly
    //    qDebug() << "[calculate_canvas_dimensions]";
    calculate_canvas_dimensions();
    //create the canvases, and of course,
    //Draw the histograms
    //    qDebug() << "[makeCanvases]";
    makeCanvases();

    //now we are ready to read from shm/socket
    //so, create the QSocket client.
    //This immediately starts looking for the server
    //set timeout to 1ms

    //server name = "vmm-mon-server" is hardcoded
    //this name should be used when creating the server
    //in the DAQ side

    //    QTimer *ttimer = new QTimer();
    //    ttimer->setSingleShot(true);
    //    connect(ttimer,SIGNAL(timeout()),this,SLOT(startClient()));
    //    ttimer->start(1);
    client = new QSocketClient(1, chambers,chips, c_chipStatistics);

    ///and TEST Fill!
    if(false)
    {
        //let's test it
        fill_counter=0;
        qDebug() << "FILL-TEST";
        timer = new QTimer();
        connect(timer, SIGNAL(timeout()), this, SLOT(FillTest()));
        timer->start(1);
    }

    update_timer = new QTimer();
    connect(update_timer, SIGNAL(timeout()), this, SLOT(UpdatePads()));
    update_timer->start(1000);

}

///FILL TEST method - Nothing operational
void MonitoringServer::FillTest()
{
    //    qDebug() << "Filling";
    //    for(int i=0;i<chambers.size();i++)
    //    {
    //        Chamber *temp_chamber = chambers.at(i);
    //        std::vector<Chip*> temp_chips = temp_chamber->getChips();

    //        for(int j=0;j<temp_chips.size();j++)
    //        {
    for(Chip *tempChip: chips)
    {
        for(int k=0;k<1;k++)//#fills per cycle is proportional
        {//to actual refill rate on GUI
            tempChip->getH_channel_statistics()->Fill(rand()%63);
            tempChip->getH_pdo_statistics()->Fill(rand()%500);
            tempChip->getH_tdo_statistics()->Fill(rand()%500);
            tempChip->getH_bcid_statistics()->Fill(rand()%4096);
            fill_counter++;
        }
    }
    //        }
    //    }


    //this makes a difference on the refill rate
    if(fill_counter%100==0)
        UpdatePads();
}
///**** CANVAS specific methods:
void MonitoringServer::UpdatePads()
{
    c_chipStatistics->ModAndUpd_Pads();
}
