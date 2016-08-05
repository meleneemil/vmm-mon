#ifndef CHAMBER_H
#define CHAMBER_H

#include <vector>

#include "chip.h"

class Chamber
{
public:
    Chamber(QString);
    void addChip(Chip*);
    std::vector<Chip*> getChips() const;
    QString getName();
    void drawChannelStatistics();
    void drawPdoStatistics();
    void drawTdoStatistics();
    void drawBCIDStatistics();
    TH1D *getH_channel_statistics() const;
    TH1D *getH_pdo_statistics() const;
    TH1D *getH_tdo_statistics() const;
    TH1D *getH_bcid_statistics() const;

private:
    QString name;
    std::vector<Chip*> chips;
    TH1D* h_channel_eventScreen;
    TH1D* h_pdo_eventScreen;
    TH1D* h_tdo_eventScreen;
    TH1D* h_bcid_eventScreen;

    TH1D* h_channel_statistics;
    TH1D* h_pdo_statistics;
    TH1D* h_tdo_statistics;
    TH1D* h_bcid_statistics;

};

#endif // CHAMBER_H
