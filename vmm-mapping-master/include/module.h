#ifndef MODULE_H
#define MODULE_H

//std/stl
#include <vector>
#include <string>

//boost
#include <boost/property_tree/ptree.hpp>

//nsw
#include "layer.h"

class Module {

    public :
        Module();
        virtual ~Module(){};

        void setDebug(bool doit) { m_dbg = doit; }
        bool dbg() { return m_dbg; }

        void setId(int id) { m_id = id; }
        int id() { return m_id; }

        // load the module
        bool load(const boost::property_tree::ptree::value_type pt);

        // does this module have the board with ID id?
        bool hasBoardId(int id);

        std::string name() { return m_name; }
        std::string type() { return m_type; }
        std::string identifier() { return m_identifier; }

        void setMapDir(std::string mapdir) { m_map_dir = mapdir; }
        std::string mapDir() { return m_map_dir; }

        std::vector<Layer>& layers() { return m_layers; }
        int nLayers() { return (int)m_layers.size(); }

    private :
        bool m_dbg;
        int m_id;
        std::string m_map_dir;
        std::string m_name;
        std::string m_type;
        std::string m_identifier;

        std::vector<Layer> m_layers;

}; // class

#endif
