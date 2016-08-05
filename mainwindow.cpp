#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    configure();

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

void MainWindow::configure()
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
    }
    else
        qDebug() << "Could not open config file.";
}

void MainWindow::calculate_canvas_dimensions()
{
    //the canvas will have a line for every chip
    canvas_size_in_y = noOfChips;
    canvas_size_in_x = Chip::getNoOfStatisticsHistos();
}



void MainWindow::makeCanvases()
{
    //the histograms are created in the Chip class
    //since each of them refers to a chip
    //no functionality yet for histograms per chamber

    //let's make one canvas to rule them all

    c_main = new QMainCanvas();
    c_main->resize(c_main->sizeHint());
    c_main->setWindowTitle("Chip Statistics");
    c_main->setGeometry( 100, 100, 700, 500 );
    c_main->show();
    c_main->Divide(canvas_size_in_x,canvas_size_in_y);

    int temp_cd=1;
    for(int i=0;i<chambers.size();i++)
    {
        Chamber *temp_chamber = chambers.at(i);
        std::vector<Chip*> temp_chips = temp_chamber->getChips();

        for(int j=0;j<temp_chips.size();j++)
        {
            Chip *tempChip = temp_chips.at(j);

            c_main->cd(temp_cd);
            tempChip->drawChannelStatistics();
            temp_cd++;
            c_main->cd(temp_cd);
            tempChip->drawPdoStatistics();
            temp_cd++;
            c_main->cd(temp_cd);
            tempChip->drawTdoStatistics();
            temp_cd++;
            c_main->cd(temp_cd);
            tempChip->drawBCIDStatistics();
            temp_cd++;
        }
    }



}
