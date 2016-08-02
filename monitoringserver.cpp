#include "monitoringserver.h"
#include <iostream>
#include <QDebug>
#include <QFile>

MonitoringServer::MonitoringServer(QWidget *parent) : QWidget(parent)
{
    qDebug() << "[MonitoringServer] New mon-server";

    //first we make the qsocketclient, to read from the socket
    //(later we can make a version to work directly on the vmm2dcs

    /**
      1) Configuration
      2) Create histos
      3) create canvases, and draw
      4) their frames should be ready
      5) show them.
      ...hmmm
      */

    //read config file
    configure();
    //calculate how many chip histos
    //and how many chambers, to Divide the
    //canvas properly
    calculate_canvas_dimensions();
    //create the canvases, and of course,
    //Draw the histograms
    makeCanvases();

    //now we are ready to read from shm/socket
    //and Fill!

/*
    //just printing to debug
    for(int i=0;i<chambers.size();i++)
    {
        Chamber *tempChamber = chambers.at(i);
        qDebug() << tempChamber->getName();
        std::vector<Chip*> tempChips = tempChamber->getChips();

        for(int j=0;j<tempChips.size();j++)
            qDebug() << (tempChips.at(j))->getName();
        std::cout << std::endl;
    }
*/
}

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
                tempchamber->addChip(list.at(i));
                noOfChips++;
            }
            noOfChambers++;
            chambers.push_back(tempchamber);

        }
        inputFile.close();
        qDebug() << "[MonitoringServer] Configuration done.";
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
