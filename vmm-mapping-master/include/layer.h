#ifndef LAYER_H
#define LAYER_H

//nsw
#include "connector.h"

//boost
#include <boost/property_tree/ptree.hpp>

//std/stl
#include <string>
#include <vector>

class Layer {

    public :
        Layer();
        virtual ~Layer(){};

        void setDebug(bool doit) { m_dbg = doit; }
        bool dbg() { return m_dbg; }

        void setId(int id) { m_id = id; } 
        int id() { return m_id; }

        void setMapDir(std::string mapdir) { m_map_dir = mapdir; }
        std::string mapDir() { return m_map_dir; }

        // load this layer
        bool load(const boost::property_tree::ptree::value_type pt);

        // does this layer have the board with ID id?
        bool hasBoardId(int id);

        std::vector<Connector>& connectors() { return m_connectors; }
        int nConnectors() { return (int)m_connectors.size(); }


    private :
        bool m_dbg;
        int m_id;
        std::string m_map_dir;

        int n_elements;

        std::vector<Connector> m_connectors;


}; // class

#endif
