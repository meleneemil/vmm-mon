#ifndef MONITORINGSERVER_H
#define MONITORINGSERVER_H
//C includes
#include <vector>
//ROOT includes
#include <TH1.h>
//"local" includes
#include "canvas.h"
#include "chamber.h"
#include "qsocketclient.h"
#include "chip.h"

class MonitoringServer : public QWidget
{
    Q_OBJECT
public:
    explicit MonitoringServer(QWidget *parent = 0);



};

#endif // MONITORINGSERVER_H
