//nsw
#include "det_config.h"

//std/stl
#include <iostream>
using namespace std;

//boost
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/foreach.hpp>
#include <boost/algorithm/string.hpp>


//////////////////////////////////////////////////////////////////////////////
// ------------------------------------------------------------------------ //
// DetectorConfig
// ------------------------------------------------------------------------ //
//////////////////////////////////////////////////////////////////////////////
DetectorConfig::DetectorConfig() :
    m_dbg(false),
    m_map_dir(""),
    m_name("")
{
}
// ------------------------------------------------------------------------ //
bool DetectorConfig::loadDetectorSetup(string filename)
{
    bool ok = true;

    using boost::property_tree::ptree;
    using namespace boost::property_tree::xml_parser;
    ptree pt;

    read_xml(filename, pt, trim_whitespace | no_comments);


    try {
        // setup name
        m_name = pt.get_child("daq_configuration").get<string>("name");

        BOOST_FOREACH(const ptree::value_type &v, pt.get_child("daq_configuration")) {

            // load a detector / sector
            if(v.first == "detector") {
                int id = std::stoi(v.second.get<string>("<xmlattr>.id"));
                Detector tmpDetector;
                tmpDetector.setId(id);
                tmpDetector.setMapDir(m_map_dir);
                if(!tmpDetector.load(v)) ok = false;
                if(ok) m_detectors.push_back(tmpDetector);
            }

        } // foreach

    } // try
    catch(std::exception &e) {
        cout << "DetectorConfig::loadDetectorSetup    ERROR: " << e.what() << endl;
        ok = false;

    } // catch

    


    return ok;
}


