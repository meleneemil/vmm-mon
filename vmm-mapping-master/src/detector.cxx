//nsw
#include "detector.h"

//std/stl
#include <iostream>
using namespace std;

//boost
#include <boost/foreach.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/algorithm/string.hpp>

//////////////////////////////////////////////////////////////////////////////
// ------------------------------------------------------------------------ //
// Detector
// ------------------------------------------------------------------------ //
//////////////////////////////////////////////////////////////////////////////
Detector::Detector() :
    m_dbg(false),
    m_id(-1),
    m_map_dir(""),
    m_name(""),
    m_identifier("")
{
    m_modules.clear();
}
// ------------------------------------------------------------------------ //
bool Detector::load(const boost::property_tree::ptree::value_type pt)
{
    bool ok = true;
    using boost::property_tree::ptree;
    using namespace boost;

    try {
        BOOST_FOREACH(const ptree::value_type &v, pt.second) {

            ///////////////////////////////////////////
            // detector name
            ///////////////////////////////////////////
            if(v.first == "name") {
                m_name = v.second.data();
                trim(m_name);
            }

            ///////////////////////////////////////////
            // load module
            ///////////////////////////////////////////
            else if(v.first == "module") {
                int id = std::stoi(v.second.get<string>("<xmlattr>.id"));
                Module tmpModule;
                tmpModule.setId(id);
                tmpModule.setMapDir(m_map_dir);
                if(!tmpModule.load(v)) ok = false;
                if(ok) m_modules.push_back(tmpModule);
            }
        } //foreach



    } // try
    catch(std::exception &e) {
        cout << "Detector::load    ERROR " << e.what() << endl;
         ok = false;
    } // catch


    return ok;
}
// ------------------------------------------------------------------------ //
bool Detector::hasBoardId(int id)
{
    bool hasit = false;
    for(auto& module : modules()) {
        if(module.hasBoardId(id)) { 
            hasit = true;
            break;
        }
    }
    return hasit;
}

