#ifndef CONNECTOR_H
#define CONNECTOR_H

//nsw
#include "board.h"

//boost
#include <boost/property_tree/ptree.hpp>

//std/stdl
#include <vector>
#include <string>
#include <tuple>

class Connector {

    public :
        Connector();
        virtual ~Connector(){};

        void setDebug(bool doit) { m_dbg = doit; }
        bool dbg() { return m_dbg; }

        void setId(int id) { m_id = id; }
        int id() { return m_id; }

        void setMapDir(std::string mapdir) { m_map_dir = mapdir; }
        std::string mapDir() { return m_map_dir; }

        // load the connector
        bool load(const boost::property_tree::ptree::value_type pt);

        std::string type() { return m_type; }
        std::string elementType() { return m_element_type; }
        std::string elementTag() { return m_element_tag; }

        bool checkMapFile(std::string mapfile, std::string mapnode);

        std::string elxMapName() { return m_elx_map_name; }
        std::string chamberMapName() { return m_chamber_map_name; }

        bool boardLoaded() { return m_board_loaded; }
        Board& board() { return m_board; }

        bool hasBoardId(int id);

        bool fillElxMap();
        bool fillChamberMap();

        std::vector<std::tuple<int, int, int> >& elxMap() { return m_elx_map; }
        std::vector<std::tuple<int, int, int> >& chamberMap() { return m_chamber_map; }

    private :
        bool m_dbg;
        int m_id;
        std::string m_map_dir;

        std::string m_type;
        std::string m_element_type;
        std::string m_element_tag;

        std::string m_elx_map_name;
        std::string m_chamber_map_name;

        // elx map: { vmm id, vmm chan, FEB chan }
        std::vector<std::tuple<int, int, int> > m_elx_map;
        // chamber map: { FEB chan, element type, detector element # }
        // element type { MMstrip, TGCstrip, TGCwire, TGCpad }
        std::vector<std::tuple<int, int, int> > m_chamber_map;

        bool m_board_loaded;
        Board m_board;


}; // class



#endif
