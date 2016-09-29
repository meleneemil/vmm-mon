#include "chamber.h"

Chamber::Chamber(QString newname, int no_of_children)
{
    name = newname;
    noOfChips=no_of_children;

    int hitmap_bins = 64*noOfChips;

    QString t_ce = "h_channel_event "+name;
    QString t_pe = "h_pdo_event "+name;
    QString t_te = "h_tdo_event "+name;
    QString t_be = "h_bcid_event "+name;
    h_channel_eventScreen = new TH1D(t_ce.toUtf8().data(),t_ce.toUtf8().data(),64,0,hitmap_bins);
    h_pdo_eventScreen     = new TH1D(t_pe.toUtf8().data(),t_pe.toUtf8().data(),501,0,500);
    h_tdo_eventScreen     = new TH1D(t_te.toUtf8().data(),t_te.toUtf8().data(),5010,0,500);
    h_bcid_eventScreen    = new TH1D(t_be.toUtf8().data(),t_be.toUtf8().data(),4097,0,4096);

    QString t_cs = "h_channel_statistics "+name;
    QString t_ps = "h_pdo_statistics "+name;
    QString t_ts = "h_tdo_statistics "+name;
    QString t_bs = "h_bcid_statistics "+name;
    h_channel_statistics  = new TH1D(t_cs.toUtf8().data(),t_cs.toUtf8().data(),64,0,hitmap_bins);
    h_pdo_statistics      = new TH1D(t_ps.toUtf8().data(),t_ps.toUtf8().data(),501,0,500);
    h_tdo_statistics      = new TH1D(t_ts.toUtf8().data(),t_ts.toUtf8().data(),501,0,500);
    h_bcid_statistics     = new TH1D(t_bs.toUtf8().data(),t_bs.toUtf8().data(),4097,0,4096);

    h_channel_statistics->GetXaxis()->SetTitleSize(.5);
    h_pdo_statistics->GetXaxis()->SetTitleSize(.5);
    h_tdo_statistics->GetXaxis()->SetTitleSize(.5);
    h_bcid_statistics->GetXaxis()->SetTitleSize(.5);


    h_channel_statistics->SetFillStyle(3001);
    h_pdo_statistics    ->SetFillStyle(3001);
    h_tdo_statistics    ->SetFillStyle(3001);
    //h_bcid_statistics   ->SetFillStyle(3001);


    h_channel_statistics->SetFillColor(kRed);
    h_pdo_statistics    ->SetFillColor(kBlue);
    h_tdo_statistics    ->SetFillColor(kGreen);
    //h_bcid_statistics   ->SetFillColor(kViolet);

}

//void Chamber::addChip(Chip* newchip)
//{
//    chips.push_back(newchip);
//}

//std::vector<Chip*> Chamber::getChips() const
//{
//    return chips;
//}

QString Chamber::getName()
{
    return name;
}

TH1D *Chamber::getH_channel_statistics() const
{
    return h_channel_statistics;
}

TH1D *Chamber::getH_pdo_statistics() const
{
    return h_pdo_statistics;
}

TH1D *Chamber::getH_tdo_statistics() const
{
    return h_tdo_statistics;
}

TH1D *Chamber::getH_bcid_statistics() const
{
    return h_bcid_statistics;
}

void Chamber::drawChannelStatistics()
{
    h_channel_statistics->Draw();
}

void Chamber::drawPdoStatistics()
{
    h_pdo_statistics->Draw();
}

void Chamber::drawTdoStatistics()
{
    h_tdo_statistics->Draw();
}

void Chamber::drawBCIDStatistics()
{
    h_bcid_statistics->Draw();
}

void Chamber::drawChannelEvent()
{
    h_channel_eventScreen->Draw();
}

void Chamber::drawPdoEvent()
{
    h_pdo_eventScreen->Draw();
}

void Chamber::drawTdoEvent()
{
    h_tdo_eventScreen->Draw();
}

void Chamber::drawBCIDEvent()
{
    h_bcid_eventScreen->Draw();
}
