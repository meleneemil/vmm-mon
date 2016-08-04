#include "monitoringserver.h"
#include <iostream>
#include <QDebug>
#include <QFile>

MonitoringServer::MonitoringServer(QWidget *parent) : QWidget(parent)
{
    //    qDebug() << "[MonitoringServer] New mon-server";

    //first we make the qsocketclient, to read from the socket
    //(later we can make a version to work directly on the vmm2dcs

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

    update_timer= new QTimer();
    connect(update_timer,SIGNAL(timeout()),this,SLOT(UpdatePads()));
    update_timer->start(500);

    //client = new QSocketClient(5, chambers,chips, c_chipStatistics);

    ///and TEST Fill!
    if(true)
    {
        //let's test it
        fill_counter=0;
        qDebug() << "FILL-TEST";
        timer1 = new QTimer();
        timer2 = new QTimer();
        timer3 = new QTimer();
        timer4 = new QTimer();
        connect(timer1, SIGNAL(timeout()), this, SLOT(FillTest1()));
        connect(timer2, SIGNAL(timeout()), this, SLOT(FillTest2()));
        connect(timer3, SIGNAL(timeout()), this, SLOT(FillTest3()));
        connect(timer4, SIGNAL(timeout()), this, SLOT(FillTest4()));
        timer1->start(1);
        timer2->start(1);
        timer3->start(1);
        timer4->start(1);
    }


    ///here we create a 1sec timer to print the rate
    rate_timer=new QTimer();
    connect(rate_timer,SIGNAL(timeout()),this,SLOT(printRate()));
    rate_timer->start(rate_interval);//
    ///****************************************************

}
//void MonitoringServer::startClient()
//{
//    client = new QSocketClient(10);

//}

void MonitoringServer::configure()
{
    //read file, and make the vectors (chambers+chips)

    QFile inputFile("/home/ak/GIT/vmm-mon/config.txt");
    if (inputFile.open(QIODevice::ReadOnly))
    {
        QTextStream in(&inputFile);
        while (!in.atEnd())
        {
            //read each line
            QString line = in.readLine();
            if(line.size()<2)
                continue;//ignore lines with 1 or less characters.
            if(line.startsWith("#") || line.startsWith("//"))
                continue;//ignore comment lines

            //split by spaces
            QStringList list = line.split(" ");

            //first item is the chamber name
            Chamber *tempchamber = new Chamber(list.at(0));

            //each next item is a chip of that chamber
            for(int i=1;i<list.size();i++)
            {//start from 1, because 0 was the chamber name

                Chip* c = new Chip(list.at(i));
                tempchamber->addChip(c);
                chips.push_back(c);
                noOfChips++;
            }
            noOfChambers++;
            chambers.push_back(tempchamber);

        }
        inputFile.close();






        //        qDebug() << "[MonitoringServer] Configuration done.";
    }
    else
        qDebug() << "Could not open config file.";

    //here the vectors are made + the hists are created
}

void MonitoringServer::makeCanvases()
{
    //the histograms are created in the Chip class
    //since each of them refers to a chip
    //no functionality yet for histograms per chamber

    //let's make a canvas for the statistics of all chips

    c_chipStatistics = new QMainCanvas();
    c_chipStatistics->resize(c_chipStatistics->sizeHint());
    c_chipStatistics->setWindowTitle("Chip Statistics");
    c_chipStatistics->setGeometry( 100, 100, 700, 500 );
    c_chipStatistics->show();
    c_chipStatistics->Divide(canvas_size_in_x,canvas_size_in_y);

    int temp_cd=1;
    for(int i=0;i<chambers.size();i++)
    {
        Chamber *temp_chamber = chambers.at(i);
        std::vector<Chip*> temp_chips = temp_chamber->getChips();

        for(int j=0;j<temp_chips.size();j++)
        {
            Chip *tempChip = temp_chips.at(j);

            c_chipStatistics->cd(temp_cd);
            tempChip->drawChannelStatistics();
            temp_cd++;
            c_chipStatistics->cd(temp_cd);
            tempChip->drawPdoStatistics();
            temp_cd++;
            c_chipStatistics->cd(temp_cd);
            tempChip->drawTdoStatistics();
            temp_cd++;
            c_chipStatistics->cd(temp_cd);
            tempChip->drawBCIDStatistics();
            temp_cd++;
        }
    }



}
void MonitoringServer::calculate_canvas_dimensions()
{
    //the canvas will have a line for every chip
    canvas_size_in_y = noOfChips;
    canvas_size_in_x = Chip::getNoOfStatisticsHistos();
}
///FILL TEST method - Nothing operational

void MonitoringServer::FillTest1()
{
    for(Chip *tempChip: chips)
    {
        for(int k=0;k<1;k++)
        {
            tempChip->getH_channel_statistics()->Fill(rand()%63);
            //            tempChip->getH_pdo_statistics()->Fill(rand()%500);
            //            tempChip->getH_tdo_statistics()->Fill(rand()%500);
            //            tempChip->getH_bcid_statistics()->Fill(rand()%4096);
        }
    }
    fill_counter++;
}

void MonitoringServer::FillTest2()
{
    for(Chip *tempChip: chips)
    {
        for(int k=0;k<1;k++)
        {
            //            tempChip->getH_channel_statistics()->Fill(rand()%63);
            tempChip->getH_pdo_statistics()->Fill(rand()%500);
            //            tempChip->getH_tdo_statistics()->Fill(rand()%500);
            //            tempChip->getH_bcid_statistics()->Fill(rand()%4096);
        }
    }
}

void MonitoringServer::FillTest3()
{
    for(Chip *tempChip: chips)
    {
        for(int k=0;k<1;k++)
        {
            //            tempChip->getH_channel_statistics()->Fill(rand()%63);
            //            tempChip->getH_pdo_statistics()->Fill(rand()%500);
            tempChip->getH_tdo_statistics()->Fill(rand()%500);
            //            tempChip->getH_bcid_statistics()->Fill(rand()%4096);
        }
    }
}

void MonitoringServer::FillTest4()
{
    for(Chip *tempChip: chips)
    {
        for(int k=0;k<1;k++)
        {
            //            tempChip->getH_channel_statistics()->Fill(rand()%63);
            //            tempChip->getH_pdo_statistics()->Fill(rand()%500);
            //            tempChip->getH_tdo_statistics()->Fill(rand()%500);
            tempChip->getH_bcid_statistics()->Fill(rand()%4096);
        }
    }
}



///**** CANVAS specific methods:
void MonitoringServer::UpdatePads()
{
    c_chipStatistics->ModAndUpd_Pads();
}

void MonitoringServer::printRate()
{

    qDebug()<< "Rate [kHz]= "<< (fill_counter-fill_counter_1sec_ago)/((double)rate_interval);
    fill_counter_1sec_ago = fill_counter;
}
