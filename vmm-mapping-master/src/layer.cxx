//nsw
#include "layer.h"

//std/stl
#include <iostream>
#include <sstream>
using namespace std;

//boost
#include <boost/foreach.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/algorithm/string.hpp>

//////////////////////////////////////////////////////////////////////////////
// ------------------------------------------------------------------------ //
// Layer
// ------------------------------------------------------------------------ //
//////////////////////////////////////////////////////////////////////////////
Layer::Layer() :
    m_dbg(false),
    m_id(-1),
    m_map_dir(""),
    n_elements(0)
{
}
// ------------------------------------------------------------------------ //
bool Layer::load(const boost::property_tree::ptree::value_type pt)
{
    bool ok = true;
    using boost::property_tree::ptree;
    using namespace boost;

    try {

        BOOST_FOREACH(const ptree::value_type &v, pt.second) {

            ///////////////////////////////////////////
            // number of elements (strips, pads, etc...)
            ///////////////////////////////////////////
            if(v.first=="element_count") {
                stringstream elem;
                elem << v.second.data();
                string nelem = elem.str();
                trim(nelem);
                if(nelem=="") {
                    cout << "Layer::load    ERROR element_count node is \"\"" << endl;
                    ok = false;
                    break;
                }
                n_elements = std::stoi(nelem);
            }

            ///////////////////////////////////////////
            // load connector
            ///////////////////////////////////////////
            else if(v.first=="connector") {
                int id = std::stoi(v.second.get<string>("<xmlattr>.id"));
                Connector tmpConnector;
                tmpConnector.setDebug(m_dbg);
                tmpConnector.setId(id);
                tmpConnector.setMapDir(m_map_dir);
                if(!tmpConnector.load(v)) ok = false;
                if(ok) m_connectors.push_back(tmpConnector);
            }

        } // foreach

    } // try
    catch(std::exception &e) {
        cout << "Layer::load    ERROR " << e.what() << endl;
        ok = false;
    }

    return ok;
}
// ------------------------------------------------------------------------ //
bool Layer::hasBoardId(int id)
{
    bool hasit = false;
    for(auto& connector: connectors()) {
        if(connector.hasBoardId(id)) {
            hasit = true;
            break;
        }
    }
    return hasit;
}
