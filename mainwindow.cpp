#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->setupTreeWidget,SIGNAL(itemSelectionChanged()),this,SLOT(treeSelectionChanged()));


    configure();//read config file
    calculate_canvas_dimensions();//based on selected items
    makeCanvases();

    client = new QSocketClient(1, chambers,chips, c_main); //1ms is ok
    client->start();
    //    start_random_fill(); //to test filling

    startCanvasUpdates();
}

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
            QTreeWidgetItem *chamber_item=new QTreeWidgetItem((QTreeWidget*)0, QStringList(list.at(0)));

            //            insertItem(list.at(0));
            //each next item is a chip of that chamber
            for(int i=1;i<list.size();i++)
            {//start from 1, because 0 was the chamber name

                Chip* c = new Chip(list.at(i));
                tempchamber->addChip(c);
                chips.push_back(c);

                //make tree item children
                //and add them to the chamber item
                QTreeWidgetItem *chip_item=new QTreeWidgetItem((QTreeWidget*)0, QStringList(list.at(i)));
                chamber_item->addChild(chip_item);

                noOfChips++;
            }

            //add the chamber item, with its children to the tree
            ui->setupTreeWidget->insertTopLevelItem(0, chamber_item);

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
    //the canvas will have a line for every chip,board,or chamber
    //from the selected ones
    canvas_size_in_y = noOfChips;

    //this is probably 4 for chips,boards, or chambers...to be seen
    canvas_size_in_x = Chip::getNoOfStatisticsHistos();
}

void MainWindow::makeCanvases()
{
    //the histograms are created in the Chip class
    //since each of them refers to a chip
    //no functionality yet for histograms per chamber

    //let's make one canvas to rule them all
    //this canvas will be defined by what is selected in the gui
    //each line can be a chip, a board, a chamber

    //chip   :         hitmap,pdo,tdo,bcid + eventDisplay on itself
    //board  :combined hitmap,pdo,tdo,bcid + eventDisplay on itself
    //chamber:combined hitmap,pdo,tdo,bcid + eventDisplay on itself
    //so, each item=line will have 4 histos (dual, with the eventDisplay)

    c_main = new QMainCanvas();
    c_main->resize(c_main->sizeHint());
    c_main->setWindowTitle("vmm-mon Canvas");
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

void MainWindow::treeSelectionChanged()
{
    //    fixTreeSelection();
    //    updateCanvasFromSelection();

    qDebug() << "SELECTED ITEMS:";
    //selected items list
    QList<QTreeWidgetItem *> list = ui->setupTreeWidget->selectedItems();

    //if list is empty, draw everything
    if(list.size()==0)
    {

    }
    else
    {
        for(QTreeWidgetItem * s: list)
        {
            if(s->parent())
                qDebug() << s->parent()->text(0) <<" : "<<s->text(0);
            else
                qDebug()<<s->text(0);
        }
    }

}

///FILL TEST method - Nothing operational
void MainWindow::start_random_fill()
{
    //let's test it
    fill_counter=0;
    qDebug() << "FILL-TEST";
    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(FillTest()));
    timer->start(1);
}
void MainWindow::FillTest()
{
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
    //    if(fill_counter%100==0)
    //        UpdatePads();
}
///**** CANVAS Update specific methods:
void MainWindow::startCanvasUpdates()
{
    update_timer = new QTimer();
    connect(update_timer, SIGNAL(timeout()), this, SLOT(UpdatePads()));
    update_timer->start(1000);

}
void MainWindow::UpdatePads()
{
    c_main->ModAndUpd_Pads();
}
///whatevers
MainWindow::~MainWindow()
{
    delete ui;
    //    QApplication::quit();
}

void MainWindow::on_b_clearTreeSelection_released()
{
    ui->setupTreeWidget->clearSelection();
}
