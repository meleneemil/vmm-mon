#ifndef DATAHANDLER_H
#define DATAHANDLER_H

#include "chamber.h"
#include "chip.h"
#include "canvas.h"
#include <QStringList>
#include <QDebug>

class DataHandler
{
public:
    DataHandler(std::vector<Chamber*>,std::vector<Chip*>,QMainCanvas*);
    void writeDataSimple(QString);
    void fill(int,QString,int,int,int,int);
//    void fill(QString,std::vector<int>,std::vector<int>,std::vector<int>,std::vector<int>);
    void saveDataSendLater(QString);

private:
    std::vector<Chamber*> chambers;
    std::vector<Chip*> chips;
    QMainCanvas* c_main;
//    int findChamberFromChip(QString);
    int fill_counter;
    int last_trig_cnt;
    QString tmp_list1;
    std::vector<std::vector<QString>> config_table;
    std::vector<QString> config_row;

    std::vector<QString> s_chips;
    std::vector<int> s_strip;
    std::vector<int> s_pdo;
    std::vector<int> s_tdo;
    std::vector<int> s_bcid;

};

#endif // DATAHANDLER_H
