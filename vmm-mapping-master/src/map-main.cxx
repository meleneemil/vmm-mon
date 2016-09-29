//nsw
#include "map_handler.h"

#include <iostream>
using namespace std;
#include <string>

int main(int argc, char* argv[]) {

    cout << "test mapping" << endl;

    MapHandler* handler = new MapHandler();
    handler->loadDaqConfiguration("/Users/dantrim/workarea/NSW/myreadout/vmm-mapping/DAQ_config_TZ.xml");
    handler->buildMapping();


    // boardContents() returns a vector of:
    // vector<nsw_map::boardContentMap> (c.f. map_handler.h)
    // where boardContentMap:
    //  std::map<std::string, std::vector<std::string> >
    //      -> [board name][ { list of VMM names } ]
    for(auto boardidmap : handler->boardContents()) {
        for(auto boardid : boardidmap) {
            cout << "loaded board: " << boardid.first << endl;
            for(auto vmm : boardid.second) {
                cout << "    --> " << vmm << endl;
            }
        }
    }

    cout << "number of boards " << handler->numberOfBoards() << endl;
    cout << "first IP " << handler->firstIP() << endl;
    cout << "board ID from " << handler->firstIP() << ": " << handler->boardIDfromIP(handler->firstIP()) << endl;
    // name of chip on board id 0, vmm 2
    cout << "chip name [0][2] " << handler->chipName(0, 2) << endl;
    // element # for board 0, vmm id 3, vmm channel 61
    cout << "strip # for [0][3][61] : " << handler->elementNumber(0,3,61) << endl;


    
    return 0;
}
