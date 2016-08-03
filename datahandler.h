#ifndef DATAHANDLER_H
#define DATAHANDLER_H

#include "chamber.h"
#include "chip.h"

class DataHandler
{
public:
    DataHandler(std::vector<Chamber*>);
    void writeData(QString);

private:
    std::vector<Chamber*> chambers;
};

#endif // DATAHANDLER_H
