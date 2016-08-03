#include "datahandler.h"

DataHandler::DataHandler(std::vector<Chamber*> _chambers,
                         std::vector<Chip*> _chips,
                         QMainCanvas* _c_chipStatistics)
{
    fill_counter=0;
    chambers = _chambers;
    chips = _chips;
    c_chipStatistics = _c_chipStatistics;
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

    //here we do FORMAT Option 1
    //with fill strategy 1

    int strip;
    int pdo;
    int tdo;
    int bcid;
    QString chip;

    QStringList list = data.split(" ");

    //old format
    //chip  = list.at(3);
    //strip = list.at(5).toInt();
    //pdo   = list.at(9).toInt();
    //tdo   = list.at(10).toInt();
    //bcid  = list.at(12).toInt();
    //old format, simplified
    chip  = list.at(0);
    strip = list.at(1).toInt();
    pdo   = list.at(2).toInt();
    tdo   = list.at(3).toInt();
    bcid  = list.at(4).toInt();

    fill(chip,strip,pdo,tdo,bcid);
    if(fill_counter%100==0)
        c_chipStatistics->ModAndUpd_Pads();
}

void DataHandler::fill(QString chip, int strip, int pdo,int tdo, int bcid)
{
    fill_counter++;
    //    qDebug() << chip << " " << strip<< " " << pdo<< " " << tdo;

    //    int ind_chamber = findChamberFromChip(chip);

    //    if(!ind_chamber==-1)//= if chip name found
    //    {

    //    }



    for(Chip* c: chips)
    {
        if(c->getName()==chip)
        {
            //now we are in the correct Chip
            c->getH_channel_statistics()->Fill(strip);
            c->getH_pdo_statistics()->Fill(pdo);
            c->getH_tdo_statistics()->Fill(tdo);
            c->getH_bcid_statistics()->Fill(bcid);
            //and break, since we do not need to search anymore
            break;
        }

    }
}


void DataHandler::saveDataSendLater(QString data)
{

    //here we do FORMAT Option 1
    //with fill strategy 2

    QStringList list = data.split(" ");

    //old format
//    s_chips.push_back( list.at(3));
//    s_strip.push_back( list.at(5).toInt());
//    s_pdo.push_back(   list.at(9).toInt());
//    s_tdo.push_back(   list.at(10).toInt());
//    s_bcid.push_back(  list.at(12).toInt());
    //old format, but simplified
    s_chips.push_back( list.at(0));
    s_strip.push_back( list.at(1).toInt());
    s_pdo.push_back(   list.at(2).toInt());
    s_tdo.push_back(   list.at(3).toInt());
    s_bcid.push_back(  list.at(4).toInt());


    //And we will fill+ModAndUpd every 10 received fuking packets
    if(s_chips.size()==3)
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
//        if(fill_counter%100==0)
            c_chipStatistics->ModAndUpd_Pads();
    }


}

