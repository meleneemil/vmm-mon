#include "chip.h"

Chip::Chip(QString newname, Chamber* _parent_chamber, int index)
{
name = newname;
parent_chamber = _parent_chamber;
index_in_parent = index;


QString t_ce = "h_channel_event "+name+" of "+parent_chamber->getName();
QString t_pe = "h_pdo_event "+name+" of "+parent_chamber->getName();
QString t_te = "h_tdo_event "+name+" of "+parent_chamber->getName();
QString t_be = "h_bcid_event "+name+" of "+parent_chamber->getName();
h_channel_eventScreen = new TH1D(t_ce.toUtf8().data(),t_ce.toUtf8().data(),64,0,63);
h_pdo_eventScreen     = new TH1D(t_pe.toUtf8().data(),t_pe.toUtf8().data(),501,0,500);
h_tdo_eventScreen     = new TH1D(t_te.toUtf8().data(),t_te.toUtf8().data(),5010,0,500);
h_bcid_eventScreen    = new TH1D(t_be.toUtf8().data(),t_be.toUtf8().data(),4097,0,4096);

QString t_cs = "h_channel_statistics "+name+" of "+parent_chamber->getName();
QString t_ps = "h_pdo_statistics "+name+" of "+parent_chamber->getName();
QString t_ts = "h_tdo_statistics "+name+" of "+parent_chamber->getName();
QString t_bs = "h_bcid_statistics "+name+" of "+parent_chamber->getName();
h_channel_statistics  = new TH1D(t_cs.toUtf8().data(),t_cs.toUtf8().data(),64,0,63);
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

void Chip::resetAllHistos()
{

    h_channel_eventScreen->Reset();
    h_pdo_eventScreen->Reset();
    h_tdo_eventScreen->Reset();
    h_bcid_eventScreen->Reset();

    h_channel_statistics->Reset();
    h_pdo_statistics->Reset();
    h_tdo_statistics->Reset();
    h_bcid_statistics->Reset();
}

QString Chip::getName()
{
    return name;
}

Chamber* Chip::getParent()
{
    return parent_chamber;
}

int Chip::getIndex()
{
    return index_in_parent;
}

int Chip::getNoOfStatisticsHistos()
{
    return noOfStatisticsHistos;
}

TH1D *Chip::getH_channel_statistics() const
{
    return h_channel_statistics;
}

TH1D *Chip::getH_pdo_statistics() const
{
    return h_pdo_statistics;
}

TH1D *Chip::getH_tdo_statistics() const
{
    return h_tdo_statistics;
}

TH1D *Chip::getH_bcid_statistics() const
{
    return h_bcid_statistics;
}

TH1D *Chip::getH_channel_eventScreen() const
{
    return h_channel_eventScreen;
}

TH1D *Chip::getH_pdo_eventScreen() const
{
    return h_pdo_eventScreen;
}

TH1D *Chip::getH_tdo_eventScreen() const
{
    return h_tdo_eventScreen;
}

TH1D *Chip::getH_bcid_eventScreen() const
{
    return h_bcid_eventScreen;
}

void Chip::drawChannelStatistics()
{
    h_channel_statistics->Draw();
}

void Chip::drawPdoStatistics()
{
    h_pdo_statistics->Draw();
}

void Chip::drawTdoStatistics()
{
    h_tdo_statistics->Draw();
}

void Chip::drawBCIDStatistics()
{
    h_bcid_statistics->Draw();
}

void Chip::drawChannelEvent()
{
    h_channel_eventScreen->Draw();
}

void Chip::drawPdoEvent()
{
    h_pdo_eventScreen->Draw();
}

void Chip::drawTdoEvent()
{
    h_tdo_eventScreen->Draw();
}

void Chip::drawBCIDEvent()
{
    h_bcid_eventScreen->Draw();
}
