#include "datahandler.h"

DataHandler::DataHandler(std::vector<Chamber*> _chambers,
                         std::vector<Chip*> _chips,
                         QMainCanvas* _c_main)
{
    fill_counter=0;
    chambers = _chambers;
    chips = _chips;
    c_main = _c_main;
}

void DataHandler::writeDataSimple(QString data)
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
        qDebug() << "in config";
        if(tmp_list1=="start")
        {
            config_table.clear();
        }
        else if(tmp_list1 == "end")
        {
            //            sendConfigToMainWindow();
            for(std::vector<QString> c: config_table)
                for(QString qq: c)
                    qDebug() << qq;


        }
        else
        {
            list.removeFirst();
            for(QString ss: list)
                config_row.push_back(ss);

            config_table.push_back(config_row);
            config_row.clear();
        }

    }
    else//normal data packet
    {
        //old format, simplified

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

    }
}

void DataHandler::fill(int trig_cnt, QString chip, int strip, int pdo,int tdo, int bcid)
{
    //        qDebug() << "Filling with: "<<chip<<" "<<strip<<" "<<pdo;
    //    qDebug() << "TRIG = "<<trig_cnt;


    fill_counter++;
    for(Chip* c: chips)
    {
        if(c->getName()==chip)
        {
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
                //             last_trig_cnt = trig_cnt;
            }
            c->getH_channel_eventScreen()-> Fill(strip);
            c->getH_pdo_eventScreen()    -> Fill(pdo);
            c->getH_tdo_eventScreen()    -> Fill(tdo);
            c->getH_bcid_eventScreen()   -> Fill(bcid);


            //and break, since we do not need to search anymore
            break;
        }

    }
    last_trig_cnt=trig_cnt;
}


void DataHandler::saveDataSendLater(QString data)
{

    //here we do FORMAT Option 2
    //with fill strategy 2

    QStringList list = data.split(" ");

    //old format, but simplified
    //size must be a multiple of 5!!
    for(int i=0;i<list.size();i+=6)
    {
        s_chips.push_back( list.at(i+1));
        s_strip.push_back( list.at(i+2).toInt());
        s_pdo.push_back(   list.at(i+3).toInt());
        s_tdo.push_back(   list.at(i+4).toInt());
        s_bcid.push_back(  list.at(i+5).toInt());
    }
    //And we will fill+ModAndUpd every 10 received fuking packets
    if(s_chips.size()>200)
    {
        for(int i=0;i<s_chips.size();i++)
        {

            fill(0,//TODO: FIX THIS IF YOU USE IT #WARNING FIX THIS IF YOU USE IT
                 s_chips.at(i),
                 s_strip.at(i),
                 s_pdo.at(  i),
                 s_tdo.at(  i),
                 s_bcid.at( i)
                 );
        }

        s_chips.clear();
        s_pdo.clear();
        s_tdo.clear();
        s_bcid.clear();

        //perhaps this will not be optimal yet
        //=>having the ModAndUpd every 10fills...to be seen
        //        if(fill_counter<100)
        //            c_main->ModAndUpd_Pads();
        //        else if(fill_counter%1000==0)
        //            c_main->ModAndUpd_Pads();
    }


}

