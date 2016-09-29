#ifndef MAP_HANDLER_H
#define MAP_HANDLER_H

////////////////////////////////////////////////
//
// map_handler
//
// Tool that builds detector and electronics
// mapping given the provided configuration
// XML files
//
//
// daniel.joseph.antrim@cern.ch
// University of California, Irvine
// September 2016
//
///////////////////////////////////////////////

//nsw
class DaqConfiguration;
#include "element.h"

//std/stl
#include <string>
#include <unordered_map>
#include <map>
#include <vector>


#warning MAP TYPES COMMENTED OUT
namespace nsw_map {
    // { FEB CHANNEL : ELEMENT }
    typedef std::unordered_map<int, Element> febChanToElementMap;
    // { FEB ID : { febChanToElementMap } }
    typedef std::unordered_map<int, febChanToElementMap> febIdToChannelMap;

    // container type for building up list of board > [chip] needed
    // for the monitoring to build up its tree
    // { BOARD NAME : vector<VMMNAME> }
    typedef std::map<std::string, std::vector<std::string> > boardContentMap;

}


class MapHandler
{

    public :
        MapHandler();
        virtual ~MapHandler(){};

        void setDebug(bool doit) { m_dbg = doit; }
        bool dbg() { return m_dbg; }

        bool loadDaqConfiguration(std::string filename);
        DaqConfiguration& config() { return *m_daqConfig; }

        bool buildMapping();
        bool mapLoaded() { return m_map_loaded; }

        ///////////////////////////////////////////////
        // convenience methods
        ///////////////////////////////////////////////

        std::vector<nsw_map::boardContentMap>& boardContents() { return m_board_contents; }
        void sortBoardContents();
        static int getBoardIdFromName(std::string boardNameStr);

        // return the IP for the board with id 0
        std::string firstIP();


        // get the board ID from the packet IP
        int boardIDfromIP(std::string ip_string);

        // obtain the board channel provided the vmm id, vmm channel
        int boardChannel(int boardId, int vmmId, int vmmChan);

        // obtain the element number provided the board ID, vmm id, vmm channel
        int elementNumber(int boardId, int vmmId, int vmmChan);

        // get the name of the VMM on board boardID_ with vmm id ID_
        std::string chipName(int boardID_, int ID_);

        // get the number of front end boards (that have separate IPs)
        int numberOfBoards();


    private :
        bool m_dbg;
        bool m_initialized;
        bool m_map_loaded;

        DaqConfiguration* m_daqConfig;

        bool isValidBoardId(int id);

        nsw_map::febIdToChannelMap m_daq_map;

        std::vector<nsw_map::boardContentMap> m_board_contents;

}; // class



#endif
