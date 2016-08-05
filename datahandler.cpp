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

    ///here we do FORMAT Option 2
    //with fill strategy 1

    QStringList list = data.split(" ");

    //old format, simplified

    //size must be a multiple of 5!!
    for(int i=0;i<list.size();i+=5)
    {
        fill(list.at(i),
             list.at(i+1).toInt(),
             list.at(i+2).toInt(),
             list.at(i+3).toInt(),
             list.at(i+4).toInt()
             );
    }
    //    if(fill_counter%100==0)
    //        c_main->ModAndUpd_Pads();
}

void DataHandler::fill(QString chip, int strip, int pdo,int tdo, int bcid)
{
    //    qDebug() << "Filling with: "<<chip<<" "<<strip<<" "<<pdo;

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
            c->getParent->getH_channel_statistics()->Fill(strip+64*c->getIndex());
            c->getParent->getH_pdo_statistics()->Fill(pdo);
            c->getParent->getH_tdo_statistics()->Fill(tdo);
            c->getParent->getH_bcid_statistics()->Fill(bcid);


            //and break, since we do not need to search anymore
            break;
        }

    }
}


void DataHandler::saveDataSendLater(QString data)
{

    //here we do FORMAT Option 2
    //with fill strategy 2

    QStringList list = data.split(" ");

    //old format, but simplified
    //size must be a multiple of 5!!
    for(int i=0;i<list.size();i+=5)
    {
        s_chips.push_back( list.at(i));
        s_strip.push_back( list.at(i+1).toInt());
        s_pdo.push_back(   list.at(i+2).toInt());
        s_tdo.push_back(   list.at(i+3).toInt());
        s_bcid.push_back(  list.at(i+4).toInt());
    }
    //And we will fill+ModAndUpd every 10 received fuking packets
    if(s_chips.size()>200)
    {
        for(int i=0;i<s_chips.size();i++)
        {

            fill(s_chips.at(i),
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

