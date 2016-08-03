#include "datahandler.h"

DataHandler::DataHandler(std::vector<Chamber*> _chambers)
{
chambers = _chambers;
}

void DataHandler::writeData(QString data)
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

    int strip;
    int pdo;
    int tdo;
    int bcid;

//    QStringList list = data.split(" ");

//    strip = list.at(5).toInt();
//    pdo   = list.at(10).toInt();
//    tdo   = list.at(11).toInt();
//    bcid  = list.at(13).toInt();

}
