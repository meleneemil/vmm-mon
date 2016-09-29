#ifndef DETECTOR_H
#define DETECTOR_H

///////////////////////////////////////////
//
// Container class for holding a detector
// object. A "detector" in this case can 
// be seen as, for example, a MM sector
// (something that contains modules)
//
// daniel.joseph.antrim@cern.ch
// University of California, Irvine
// September 2016
//
///////////////////////////////////////////

//nsw
#include "module.h"

//std/stl
#include <vector>
#include <string>

//boost
#include <boost/property_tree/ptree.hpp>

class Detector {

    public :
        Detector();
        virtual ~Detector(){};

        void setDebug(bool doit) { m_dbg = doit; }
        bool dbg() { return m_dbg; }

        void setId(int id) { m_id = id; }
        int id() { return m_id; }

        void setMapDir(std::string mapdir) { m_map_dir = mapdir; }
        std::string mapDir() { return m_map_dir; }

        // load this detector
        bool load(const boost::property_tree::ptree::value_type pt);

        // does this detector have the board with ID id?
        bool hasBoardId(int id);

        std::string name() { return m_name; }
        std::string identifier() { return m_identifier; }

        std::vector<Module>& modules() { return m_modules; }
        int nModules() { return (int)m_modules.size(); }

    private :
        bool m_dbg;
        int m_id;
        std::string m_map_dir;

        std::string m_name;
        std::string m_identifier;

        std::vector<Module> m_modules;

}; // class

#endif
