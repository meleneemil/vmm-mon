#include "mainwindow.h"
#include "ui_mainwindow.h"
MainWindow::MainWindow(QWidget *parent) :
    debugMode(0),
    isConfigured(0),
    m_socket_receiver(0),
    eventCount(0),
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->setupTreeWidget,SIGNAL(itemSelectionChanged()),this,SLOT(treeSelectionChanged()));

    openUdpConnection();
    createCanvas();

}
/// UDP SETUP ------------------------------------------------------------------------------------------------------------
void MainWindow::openUdpConnection()
{
    m_socket_receiver = new QUdpSocket(this);
    bool bind = m_socket_receiver->bind(QHostAddress::LocalHost, 2224);
    if(!bind) {
        qDebug() << "unable to bind socket SENDER";
        m_socket_receiver->close();
        m_socket_receiver->disconnectFromHost();
    }
    connect(m_socket_receiver, SIGNAL(readyRead()), this, SLOT(readUdpPacketData()));
}
void MainWindow::readUdpPacketData(){

    while(m_socket_receiver->hasPendingDatagrams())
    {
        incomingDatagram.resize(m_socket_receiver->pendingDatagramSize());
        m_socket_receiver->readDatagram(incomingDatagram.data(),
                                        incomingDatagram.size(),
                                        &fromIP);

        if(!isPaused)
            writeDataSimple(QString(incomingDatagram));
    } // while
    eventCount++;
}
/// CANVAS SETUP ------------------------------------------------------------------------------------------------------------
void MainWindow::createCanvas()
{
    //just initializing 2 canvases (statistics+event display)
    c_main = new QMainCanvas();
    c_main->resize(c_main->sizeHint());
    c_main->setWindowTitle("vmm-mon Statistics");
    c_main->setGeometry( 100, 100, 700, 500 );
    c_main->show();

    c_event = new QMainCanvas();
    c_event->resize(c_event->sizeHint());
    c_event->setWindowTitle("vmm-mon Event Display");
    c_event->setGeometry( 150, 150, 700, 500 );
    c_event->show();

}
void MainWindow::setupCanvas()
{
    ///Divide dimensions are init'd here, and adjusted in treeSelectionChanged()
    //the canvas will have a line for every chip,board,or chamber
    //from the selected ones
    //    canvas_size_in_y = chips.size();
    //this is probably 4 for chips,boards, or chambers...to be seen
    //    canvas_size_in_x = Chip::getNoOfStatisticsHistos();

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
    c_main->Divide(canvas_size_in_x,canvas_size_in_y);
    c_event->Divide(canvas_size_in_x,canvas_size_in_y);

    if(ui->setupTreeWidget->selectedItems().size()==0)
    {//list selection is empty
        drawAllChips();
    }
    else
    {
        drawSelectedItems();
    }



}
/// DATA HANDLING ------------------------------------------------------------------------------------------------------------
void MainWindow::writeDataSimple(QString data)
{
    /*
 Here we receive the data from the SharedMemory/QSocket/other class.
 The incoming data will be events' data, split with spaces

 #########   FORMATS ############
Option 1) we do it as before, one hit = one packet

    FORMAT:"42117 TZ2 0 0 X 133 1 20 60 1123 634 146 597"

Option 2) many hits = one packet, splitting with '#' for example.

    FORMAT:"42117 TZ2 0 0 X 133 1 20 60 1123 634 146 597#42117 TZ2 0 0 X 133 1 20 60 1123 634 146 597#42117 TZ2 0 0 X 133 1 20 60 1123 634 146 597"


 #########   FILL strategy  ############

Option 1) Do it normally. Whatever we receive, we fill.

Option 2) This is for optimizing for high rates.
          We can save the data and Fill every 10 hits for example.

*/

    ///here we do FORMAT Option 2 (one packet, many hits, but even if one, doesn't matter)
    //with fill strategy 1

    QStringList list = data.split(" ");
    //    qDebug() << "list size = "<<list.size();
    tmp_list1 = list.at(1);
    //tree config packet
    if(list.at(0)=="config")
    {
        debug("in config");
        if(tmp_list1=="start")
        {
            ui->l_configd->setText("<font color='red'>NOT configured!</font>");

            deleteAllHistos();//to prepare for new config
            ui->setupTreeWidget->clear();
            chambers.clear();
            chips.clear();
            config_table.clear();
        }
        else if(tmp_list1 == "end")
        {
            //at the end of the config packets
            //and just run setupCanvas
            //then it should refresh by itself

            setupConfigLists();
            startCanvasUpdates();
            treeSelectionChanged();//trigger this to setup correctly

            ui->l_configd->setText("<font color='green'>Is configured!</font>");

            isConfigured = true;
            //            for(std::vector<QString> c: config_table)
            //                for(QString qq: c)
            //                    qDebug() << qq;
        }
        else
        {
            list.removeFirst();
            for(QString ss: list)
                config_row.push_back(ss);

            config_table.push_back(config_row);
            config_row.clear();
        }

    }//if config msg
    else if(isConfigured)//normal data packet, and config=received
    {
        //old format, simplified

        if(list.size()%6 != 0)
        {
            qDebug() << "strange packets from DAQ";
        }
        else
        {
            //size must be a multiple of 6!!
            for(int i=0;i<list.size();i+=6)
            {
                fill(list.at(i).toInt(),
                     list.at(i+1),
                     list.at(i+2).toInt(),
                     list.at(i+3).toInt(),
                     list.at(i+4).toInt(),
                     list.at(i+5).toInt()
                     );
            }

        }//else if (normal packets)
    }
    void MainWindow::setupConfigLists()
    {

        for(std::vector<QString> configLine: config_table)
        {

            Chamber *tempchamber = new Chamber(configLine.at(0),configLine.size()-1);
            QTreeWidgetItem *chamber_item=new QTreeWidgetItem((QTreeWidget*)0, QStringList(configLine.at(0)));

            for(int i=1;i<configLine.size();i++)
            {//start from 1, because 0 was the chamber name

                //see Chip constructor
                //we give it its name
                //pointer to its parent chamber
                //its index in the parent, to merge vmms
                Chip* c = new Chip(configLine.at(i),tempchamber,i-1);
                chips.push_back(c);

                //make tree item children
                //and add them to the chamber item
                QTreeWidgetItem *chip_item=new QTreeWidgetItem((QTreeWidget*)0, QStringList(configLine.at(i)));
                chamber_item->addChild(chip_item);


            }//for each name in configLine, except the first

            //add the chamber item, with its children to the tree
            ui->setupTreeWidget->insertTopLevelItem(0, chamber_item);

            chambers.push_back(tempchamber);
        }//for each configLine (each chamber)

        //    printInfo();

    }
    void MainWindow::fill(int trig_cnt, QString chip, int strip, int pdo,int tdo, int bcid)
    {
        fill_counter++;
        for(Chip* c: chips)
        {
            if(c->getName()==chip)
            {

                //            qDebug() << "Filling with: "<<chip<<" "<<strip<<" "<<pdo;
                //now we are in the correct Chip
                c->getH_channel_statistics()->Fill(strip);
                c->getH_pdo_statistics()->Fill(pdo);
                c->getH_tdo_statistics()->Fill(tdo);
                c->getH_bcid_statistics()->Fill(bcid);

                //also fill the parent of the chip
                c->getParent()->getH_channel_statistics()->Fill(strip+64*c->getIndex());
                c->getParent()->getH_pdo_statistics()->Fill(pdo);
                c->getParent()->getH_tdo_statistics()->Fill(tdo);
                c->getParent()->getH_bcid_statistics()->Fill(bcid);

                //            qDebug() << "trig = "<<trig_cnt<<" // last_trig = "<<last_trig_cnt;
                //also fill the event display histos (and reset if new event)
                if(!(trig_cnt == last_trig_cnt))
                {
                    //                qDebug() << "Resetting";
                    c->getH_channel_eventScreen()->Reset();
                    c->getH_pdo_eventScreen()    ->Reset();
                    c->getH_tdo_eventScreen()    ->Reset();
                    c->getH_bcid_eventScreen()   ->Reset();

                    c->getParent()->getH_channel_event()->Reset();
                    c->getParent()->getH_pdo_event()    ->Reset();
                    c->getParent()->getH_tdo_event()    ->Reset();
                    c->getParent()->getH_bcid_event()   ->Reset();
                    //             last_trig_cnt = trig_cnt;
                }
                c->getH_channel_eventScreen()-> Fill(strip);
                c->getH_pdo_eventScreen()    -> Fill(pdo);
                c->getH_tdo_eventScreen()    -> Fill(tdo);
                c->getH_bcid_eventScreen()   -> Fill(bcid);

                c->getParent()->getH_channel_event()->Fill(strip+64*c->getIndex());
                c->getParent()->getH_pdo_event()->Fill(pdo);
                c->getParent()->getH_tdo_event()->Fill(tdo);
                c->getParent()->getH_bcid_event()->Fill(bcid);


                //and break, since we do not need to search anymore
                break;
            }

        }
        last_trig_cnt=trig_cnt;
    }
    /// DRAWING FUNCTIONS ------------------------------------------------
    void MainWindow::drawAllChips()
    {
        debug(__FUNCTION__);
        int temp_cd=1;
        for(Chip *tempChip: chips)
        {
            c_main->cd(temp_cd);
            tempChip->drawChannelStatistics();
            c_event->cd(temp_cd);
            tempChip->drawChannelEvent();
            temp_cd++;
            c_main->cd(temp_cd);
            tempChip->drawPdoStatistics();
            c_event->cd(temp_cd);
            tempChip->drawPdoEvent();
            temp_cd++;
            c_main->cd(temp_cd);
            tempChip->drawTdoStatistics();
            c_event->cd(temp_cd);
            tempChip->drawTdoEvent();
            temp_cd++;
            c_main->cd(temp_cd);
            tempChip->drawBCIDStatistics();
            c_event->cd(temp_cd);
            tempChip->drawBCIDEvent();
            temp_cd++;
        }
    }
    void MainWindow::drawSelectedItems()
    {
        //selected items list
        QList<QTreeWidgetItem *> list = ui->setupTreeWidget->selectedItems();

        int temp_cd=1;
        for(QTreeWidgetItem *item: list)
        {
            Chamber *c = findChamber(item->text(0));
            if(c)
            {
                c_main->cd(temp_cd);
                c->drawChannelStatistics();
                c_event->cd(temp_cd);
                c->drawChannelEvent();

                temp_cd++;
                c_main->cd(temp_cd);
                c->drawPdoStatistics();
                c_event->cd(temp_cd);
                c->drawPdoEvent();

                temp_cd++;
                c_main->cd(temp_cd);
                c->drawTdoStatistics();
                c_event->cd(temp_cd);
                c->drawTdoEvent();

                temp_cd++;
                c_main->cd(temp_cd);
                c->drawBCIDStatistics();
                c_event->cd(temp_cd);
                c->drawBCIDEvent();
                temp_cd++;

            }
            else
            {
                Chip *tempchip = findChip(item->text(0));

                c_main->cd(temp_cd);
                tempchip->drawChannelStatistics();
                c_event->cd(temp_cd);
                tempchip->drawChannelEvent();
                temp_cd++;

                c_main->cd(temp_cd);
                tempchip->drawPdoStatistics();
                c_event->cd(temp_cd);
                tempchip->drawPdoEvent();
                temp_cd++;

                c_main->cd(temp_cd);
                tempchip->drawTdoStatistics();
                c_event->cd(temp_cd);
                tempchip->drawTdoEvent();
                temp_cd++;

                c_main->cd(temp_cd);
                tempchip->drawBCIDStatistics();
                c_event->cd(temp_cd);
                tempchip->drawBCIDEvent();
                temp_cd++;
            }

        }

    }
    void MainWindow::resetAllHistos()
    {
        for(Chamber *c: chambers)
            c->resetAllHistos();
        for(Chip *c: chips)
            c->resetAllHistos();
    }
    void MainWindow::deleteAllHistos()
    {
        for(Chamber *c: chambers)
            c->deleteAllHistos();
        for(Chip *c: chips)
            c->deleteAllHistos();
    }
    /// CANVAS UPDATE CONTROL --------------------------------------------------------------------------------------------------
    void MainWindow::startCanvasUpdates()
    {
        //    update_timer = new QTimer();
        //    connect(update_timer, SIGNAL(timeout()), this, SLOT(updatePads()));
        //    update_timer->start(1000);
        //---------------------------------------
        mainC_update_timer = new QTimer();
        connect(mainC_update_timer, SIGNAL(timeout()), this, SLOT(mainC_updatePads()));
        mainC_update_timer->start(1000);
        //    //---------------------------------------
        eventC_update_timer = new QTimer();
        connect(eventC_update_timer, SIGNAL(timeout()), this, SLOT(eventC_updatePads()));
        eventC_update_timer->start(500);

    }
    void MainWindow::stopCanvasUpdates()
    {
        //    update_timer->stop();
        mainC_update_timer->stop();
        eventC_update_timer->stop();
    }
    void MainWindow::updatePads()
    {
        c_main->ModAndUpd_Pads();
        c_event->ModAndUpd_Pads();
    }
    void MainWindow::mainC_updatePads()
    {
        c_main->ModAndUpd_Pads();
    }
    void MainWindow::eventC_updatePads()
    {
        c_event->ModAndUpd_Pads();
    }
    /// UI CONTROL ---------------------------------------------------------------------------------------------
    void MainWindow::treeSelectionChanged()
    {
        ///stop the updates
        stopCanvasUpdates();
        c_main->clear();
        c_event->clear();

        //selected items list
        QList<QTreeWidgetItem *> list = ui->setupTreeWidget->selectedItems();

        //if list is empty, draw everything
        if(list.size()==0)
        {
            //qDebug() << "Empty tree selection";
            canvas_size_in_y = chips.size();
            canvas_size_in_x = Chip::getNoOfStatisticsHistos();
        }
        else
        {
            //        qDebug() << "SELECTED ITEMS:";

            //        for(QTreeWidgetItem * s: list)
            //        {
            //            if(s->parent())
            //                qDebug() << s->parent()->text(0) <<" : "<<s->text(0);
            //            else
            //                qDebug()<<s->text(0);
            //        }

            //that way we can monitor a board/chamber and a specific chip at the same time
            canvas_size_in_y = list.size();
            canvas_size_in_x = Chip::getNoOfStatisticsHistos();
        }

        //redo the setup
        setupCanvas();
        ///restart updates
        startCanvasUpdates();
    }
    /// TOOLS ----------------------------------------------------------------------------------------------------------------
    Chamber* MainWindow::findChamber(QString chname)
    {
        for(Chamber *c:chambers)
            if(c->getName()==chname)
                return c;
        return NULL;
    }
    Chip* MainWindow::findChip(QString chname)
    {
        for(Chip *c:chips)
            if(c->getName()==chname)
                return c;
        return NULL;
    }
    void MainWindow::printInfo()
    {
        qDebug() << "------------------";
        for(int i=0;i<chambers.size();i++)
        {
            Chamber* c=chambers.at(i);
            qDebug()<<c->getName();
            for(int j=0;j<chips.size();j++)
            {
                Chip* cc=chips.at(j);
                qDebug()<<cc->getName();
            }
        }
        qDebug() << "------------------";
    }
    void MainWindow::debug(QString s)
    {
        if(debugMode)
            qDebug() << s;
    }
    /// WHATEVERS -------------------------------------------------------------------------------------------------------------
    //MainWindow::~MainWindow() {delete ui;}
    void MainWindow::on_showStatisticsCheckBox_stateChanged(int arg1)
    {
        if(ui->showStatisticsCheckBox->isChecked())
            c_main->show();
        else
            c_main->hide();
    }
    void MainWindow::on_showEventCheckBox_stateChanged(int arg1)
    {
        if(ui->showEventCheckBox->isChecked())
            c_event->show();
        else
            c_event->hide();
    }
    void MainWindow::on_b_Reset_released()
    {
        resetAllHistos();
    }
    void MainWindow::on_b_Pause_released()
    {
        if(isPaused)
        {
            isPaused = false;
            ui->b_Pause->setText("Pause");
        }
        else
        {
            isPaused = true;
            ui->b_Pause->setText("Resume");
        }

    }

    void MainWindow::on_b_clearTreeSelection_released()
    {
        ui->setupTreeWidget->clearSelection();
    }
