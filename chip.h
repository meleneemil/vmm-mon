#ifndef CHIP_H
#define CHIP_H

#include <QString>
#include <TH1D.h>

class Chip
{
public:
    Chip(QString);
    QString getName();
    void drawChannelStatistics();
    void drawPdoStatistics();
    void drawTdoStatistics();
    void drawBCIDStatistics();

    static int getNoOfStatisticsHistos();

    TH1D *getH_channel_statistics() const;
    TH1D *getH_pdo_statistics() const;
    TH1D *getH_tdo_statistics() const;
    TH1D *getH_bcid_statistics() const;

private:
    TH1D* h_channel_eventScreen;
    TH1D* h_pdo_eventScreen;
    TH1D* h_tdo_eventScreen;
    TH1D* h_bcid_eventScreen;

    TH1D* h_channel_statistics;
    TH1D* h_pdo_statistics;
    TH1D* h_tdo_statistics;
    TH1D* h_bcid_statistics;
    QString name;
    static int const noOfStatisticsHistos=4;
};

#endif // CHIP_H
