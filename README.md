# vmm-mon
contact: **aikoulou@cern.ch**

Qt application to monitor VMM chips, through the DAQ software found at https://gitlab.cern.ch/NSWelectronics/vmm_readout_software

## Pre-requisites:

1. Qt 5.5 or greater
2. ROOT 5.34

## Installation:
```
qmake vmm-mon.pro
make
```
## Use:
```
./vmm-mon
```
After this, the software expects some Configuration messages, the format of which is described below.
This is taken care of by the VMMDAQ or your software.
A label will indicate that the monitoring is configured, and is ready to receive data.

## Configuration:

The following string messages show one config cycle:
```
"config start"
"config MMFE1 VMM1 VMM2"
"config MMFE2 VMM3 VMM4"
"config MMFE3 VMM5 VMM6"
"config end"
```
Between config start and config end you can put any number of config messages.
The first word after config is the board/chamber, and all the next ones are the VMM chips thereof.
Each VMM name must be different.

Sending configuration resets the histograms

## Expected data format

Data is sent again as string messages.
```
message format = "trigger count, vmm name, strip, PDO, TDO, BCID"
example message: "322 VMM1.0 34 450 340 2056"
```
Then the software goes to the corresponding VMM and fills+updates the histograms.
The same is done for the parent of the VMM (which can be a Front End board or something else).

