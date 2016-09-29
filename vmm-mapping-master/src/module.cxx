//nsw
#include "module.h"

//std/stl
#include <iostream>
using namespace std;

//boost
#include <boost/foreach.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/algorithm/string.hpp>

//////////////////////////////////////////////////////////////////////////////
// ------------------------------------------------------------------------ //
// Module
// ------------------------------------------------------------------------ //
//////////////////////////////////////////////////////////////////////////////
Module::Module() :
    m_dbg(false),
    m_id(-1),
    m_map_dir(""),
    m_name(""),
    m_type("")
{
}
// ------------------------------------------------------------------------ //
bool Module::load(const boost::property_tree::ptree::value_type pt)
{
    bool ok = true;
    using boost::property_tree::ptree;
    using namespace boost;

    try {

        BOOST_FOREACH(const ptree::value_type &v, pt.second) {

            /////////////////////////////////////
            // module name
            /////////////////////////////////////
            if(v.first=="name") {
                m_name = v.second.data();
                trim(m_name); 
            }
            /////////////////////////////////////
            // module type
            /////////////////////////////////////
            else if(v.first=="type") {
                m_type = v.second.data();
                trim(m_type);
            }
            /////////////////////////////////////
            // load layers
            /////////////////////////////////////
            else if(v.first=="layer") {
                int id = std::stoi(v.second.get<string>("<xmlattr>.id"));
                Layer tmpLayer;
                tmpLayer.setId(id);
                tmpLayer.setDebug(m_dbg);
                tmpLayer.setMapDir(m_map_dir);
                if(!tmpLayer.load(v)) ok = false;
                if(ok) m_layers.push_back(tmpLayer);
            }

        } // foreach

    } // try
    catch(std::exception& e) {
        cout << "Module::load    ERROR " << e.what() << endl;
        ok = false;
    } // catch

    return ok;
}
// ------------------------------------------------------------------------ //
bool Module::hasBoardId(int id)
{
    bool hasit = false;
    for(auto& layer : layers()) {
        if(layer.hasBoardId(id)) {
            hasit = true;
            break;
        }
    }
    return hasit;
}
