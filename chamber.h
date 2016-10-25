#ifndef CHAMBER_H
#define CHAMBER_H

#include <QString>
#include <vector>
#include <TH1D.h>

//#include "chip.h"

class Chamber
{
public:
    Chamber(QString,int);
//    void addChip(Chip*);
//    std::vector<Chip*> getChips() const;
    QString getName();
    void drawChannelStatistics();
    void drawPdoStatistics();
    void drawTdoStatistics();
    void drawBCIDStatistics();
    void drawChannelEvent();
    void drawPdoEvent();
    void drawTdoEvent();
    void drawBCIDEvent();
    TH1D *getH_channel_statistics() const;
    TH1D *getH_pdo_statistics() const;
    TH1D *getH_tdo_statistics() const;
    TH1D *getH_bcid_statistics() const;
    TH1D *getH_channel_event() const;
    TH1D *getH_pdo_event() const;
    TH1D *getH_tdo_event() const;
    TH1D *getH_bcid_event() const;

    void clearAllHistos();


private:
    QString name;
    int noOfChips;
//    std::vector<Chip*> chips;
    TH1D* h_channel_event;
    TH1D* h_pdo_event;
    TH1D* h_tdo_event;
    TH1D* h_bcid_event;

    TH1D* h_channel_statistics;
    TH1D* h_pdo_statistics;
    TH1D* h_tdo_statistics;
    TH1D* h_bcid_statistics;

};

#endif // CHAMBER_H
