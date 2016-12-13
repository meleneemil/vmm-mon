#include <QLayout>
#include <QTimer>
#include <QPaintEvent>
#include <QResizeEvent>
#include <QMouseEvent>
#include <QMessageBox>

#include <stdlib.h>

#include <TCanvas.h>
#include <TVirtualX.h>
#include <TSystem.h>
#include <TStyle.h>
#include <TFormula.h>
#include <TF1.h>
#include <TH1.h>
#include <TFrame.h>
#include <TTimer.h>
#include "canvas.h"
/*
//______________________________________________________________________________
QMainCanvas::QMainCanvas(QWidget *parent) : QWidget(parent, 0), canvas(0)
{
    // QMainCanvas constructor.

    // set options needed to properly update the canvas when resizing the widget
    // and to properly handle context menus and mouse move events
    setAttribute(Qt::WA_PaintOnScreen, true);
    setAttribute(Qt::WA_OpaquePaintEvent, true);
    setMinimumSize(300, 200);
    setUpdatesEnabled(kFALSE);
    setMouseTracking(kTRUE);

    // register the QWidget in TVirtualX, giving its native window id
    int wid = gVirtualX->AddWindow((ULong_t)winId(), 600, 400);
    // create the ROOT TCanvas, giving as argument the QWidget registered id
    canvas = new TCanvas("Root Canvas", width(), height(), wid);
}
*/
//______________________________________________________________________________
QMainCanvas::QMainCanvas(QWidget *parent) : QWidget(parent)
{
    //make simple layout for the single canvas
//    QVBoxLayout *l = new QVBoxLayout(this);
    //add canvas widget to the layout

    // register the QWidget in TVirtualX, giving its native window id
    int wid = gVirtualX->AddWindow((ULong_t)winId(), 600, 400);
    // create the ROOT TCanvas, giving as argument the QWidget registered id
    canvas = new TCanvas("Root Canvas", 200, 200, wid);
    //this timer is very important
    //every time it times-out, it tells the
    //root engine to process the events.
    fRootTimer = new QTimer( this );
    QObject::connect( fRootTimer, SIGNAL(timeout()), this, SLOT(handle_root_events()) );
    fRootTimer->start( 20 );
    //set dimensions of stats box (~legend) in hists
//    gStyle->SetStatW(0.3);
//    gStyle->SetStatH(0.4);
    //make histo title larger
//    gStyle->SetTitleFontSize(0.1);
//    gStyle->SetLabelSize(0.9);
}
//______________________________________________________________________________
void QMainCanvas::handle_root_events()
{
    //ModAndUpd();
    //call the inner loop of ROOT
    gSystem->ProcessEvents();
}
//______________________________________________________________________________
void QMainCanvas::ModAndUpd()
{
    canvas->Modified();
    canvas->Update();
}

//______________________________________________________________________________
void QMainCanvas::mouseMoveEvent(QMouseEvent *e)
{
    // Handle mouse move events.
    if (canvas) {
        if (e->buttons() & Qt::LeftButton) {
            canvas->HandleInput(kButton1Motion, e->x(), e->y());
        } else if (e->buttons() & Qt::MidButton) {
            canvas->HandleInput(kButton2Motion, e->x(), e->y());
        } else if (e->buttons() & Qt::RightButton) {
            canvas->HandleInput(kButton3Motion, e->x(), e->y());
        } else {
            canvas->HandleInput(kMouseMotion, e->x(), e->y());
        }
    }
}
//______________________________________________________________________________
void QMainCanvas::mousePressEvent( QMouseEvent *e )
{
    // Handle mouse button press events.
    if (canvas) {
        switch (e->button()) {
        case Qt::LeftButton :
            canvas->HandleInput(kButton1Down, e->x(), e->y());
            break;
        case Qt::MidButton :
            canvas->HandleInput(kButton2Down, e->x(), e->y());
            break;
        case Qt::RightButton :
            // does not work properly on Linux...
            // ...adding setAttribute(Qt::WA_PaintOnScreen, true)
            // seems to cure the problem
            canvas->HandleInput(kButton3Down, e->x(), e->y());
            break;
        default:
            break;
        }
    }
}
//______________________________________________________________________________
void QMainCanvas::mouseReleaseEvent( QMouseEvent *e )
{
    // Handle mouse button release events.
    if (canvas) {
        switch (e->button()) {
        case Qt::LeftButton :
            canvas->HandleInput(kButton1Up, e->x(), e->y());
            break;
        case Qt::MidButton :
            canvas->HandleInput(kButton2Up, e->x(), e->y());
            break;
        case Qt::RightButton :
            // does not work properly on Linux...
            // ...adding setAttribute(Qt::WA_PaintOnScreen, true)
            // seems to cure the problem
            canvas->HandleInput(kButton3Up, e->x(), e->y());
            break;
        default:
            break;
        }
    }
}
//______________________________________________________________________________
void QMainCanvas::resizeEvent( QResizeEvent * )
{
    // Handle resize events.
    if (canvas) {
        canvas->Resize();
        canvas->Update();
    }
}
//______________________________________________________________________________
void QMainCanvas::paintEvent( QPaintEvent * )
{
    // Handle paint events.
    if (canvas) {
//        std::cout << "TEST"<<std::endl;
        canvas->Resize();
        canvas->Update();
    }
}

void QMainCanvas::ModAndUpd_Pads()
{
    //This method calls Mod and Upd
    //For each pad on the canvas.
    ///To be used only for canvases that have been Divide'd
    for(int i=1;i<=noOfPads;i++)
    {
        canvas->cd(i);//pads or i
        gPad->Modified();
        gPad->Update();

    }
}

void QMainCanvas::Divide(int x,int y)
{
    canvas->Divide(x,y);
    noOfPads=x*y;


}

void QMainCanvas::cd(int x)
{
    canvas->cd(x);
}

void QMainCanvas::clear()
{
    canvas->Clear();
}
