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

Option 1) we do it as before, one hit = one packet

    FORMAT:"42117 TZ2 0 0 X 133 1 20 60 1123 634 146 597"

Option 2) many hits = one packet

    FORMAT:"42117 TZ2 0 0 X 133 1 20 60 1123 634 146 597"


 */

}
