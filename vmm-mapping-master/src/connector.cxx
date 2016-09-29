//nsw
#include "connector.h"

//std/stl
#include <iostream>
#include <sstream>
using namespace std;

//boost
#include <boost/foreach.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/tokenizer.hpp>

//////////////////////////////////////////////////////////////////////////////
// ------------------------------------------------------------------------ //
// Connector
// ------------------------------------------------------------------------ //
//////////////////////////////////////////////////////////////////////////////
Connector::Connector() :
    m_dbg(false),
    m_id(-1),
    m_map_dir(""),
    m_type(""),
    m_element_type(""),
    m_elx_map_name(""),
    m_chamber_map_name(""),
    m_board_loaded(false)
{
    m_elx_map.clear();
    m_chamber_map.clear();
}
// ------------------------------------------------------------------------ //
bool Connector::load(const boost::property_tree::ptree::value_type pt)
{
    bool ok = true;
    using boost::property_tree::ptree;
    using namespace boost;

    try {

        BOOST_FOREACH(const ptree::value_type &v, pt.second) {

            ///////////////////////////////////////////////
            // type (MM or TGC)
            ///////////////////////////////////////////////
            if(v.first == "type") {
                m_type = v.second.data();
                trim(m_type);
            }
            ///////////////////////////////////////////////
            // element type (strips, pads, wires)
            ///////////////////////////////////////////////
            else if(v.first == "element_type") {
                m_element_type = v.second.data();
                trim(m_element_type);
            }
            ///////////////////////////////////////////////
            // maps
            ///////////////////////////////////////////////
            else if(v.first == "electronics_map") {
                string elx_map = v.second.data();
                trim(elx_map);
                if(!checkMapFile(elx_map, "electronics_map")) {
                    ok = false;
                    break;
                }
                m_elx_map_name = elx_map;
            }
            else if(v.first == "chamber_map") {
                string chamber_map = v.second.data();
                trim(chamber_map);
                if(!checkMapFile(chamber_map, "chamber_map")) {
                    ok = false;
                    break;
                }
                m_chamber_map_name = chamber_map; 
            }

        } // foreach

    } // try
    catch(std::exception &e) {
        cout << "Connector::load    ERROR " << e.what() << endl;
        ok = false;
    } // catch


    if(ok) {
        try {

            BOOST_FOREACH(const ptree::value_type &v, pt.second) {

                ///////////////////////////////////////////////
                // load board at this connector
                // (should only be one board per connector)
                ///////////////////////////////////////////////
                if(v.first == "board") {
                    if(boardLoaded()) {
                        cout << "Connector::load    ERROR Attempting to load more than 1 board at "
                             << "Connector with (id, type, element_type) = ("
                             << id() << ", " << type() << ", " << elementType() << ")" << endl; 
                        ok = false;
                        break;
                    }
                    Board tmpBoard;
                    tmpBoard.setDebug(m_dbg);
                    if(!tmpBoard.load(v)) { 
                        ok = false;
                        m_board_loaded = false;
                    }
                    if(ok) { 
                        m_board = tmpBoard;
                        m_board_loaded = true;
                    }
                }


            } // foreach

        } // try
        catch(std::exception &e) {
            cout << "Connector::load    ERROR " << e.what() << endl;
            ok = false;

        } // catch

    } // ok

    return ok;
}
// ------------------------------------------------------------------------ //
bool Connector::checkMapFile(string mapfilename, std::string mapnode)
{
    stringstream full_name;
    full_name << m_map_dir;
    string suffix = "/";
    if(!(boost::algorithm::ends_with(m_map_dir, suffix))) full_name << "/";
    full_name << mapfilename;

    string map_file = full_name.str();

    bool exists = std::ifstream(map_file).good();
    if(!exists) {
        cout << "Connector::checkMapFile    ERROR " << mapnode << " (" << map_file << ") unable to be loaded" << endl;
    }
    return exists;
}
// ------------------------------------------------------------------------ //
bool byFebChan(const std::tuple<int, int, int> &lhs,
                    const std::tuple<int, int, int> &rhs)
{
    return std::get<2>(lhs) < std::get<2>(rhs);
}
bool Connector::fillElxMap()
{
    m_elx_map.clear();
    bool ok = true;

    std::ifstream mapfile(m_elx_map_name.c_str());
    std::string line;
    int line_counter = 0;

    while(getline(mapfile, line)) {
        line_counter++;

        boost::trim(line);
        if(boost::starts_with(line, "#") || line.empty()) continue;

        typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
        boost::char_separator<char> sep(", \t");
        tokenizer tokens(line, sep);

        std::string vmm_id_str;
        std::string vmm_chan_str;
        std::string feb_chan_str;

        enum { state_vmm_id, state_vmm_chan, state_feb_chan } parse_state;

        parse_state = state_vmm_id;

        for(tokenizer::iterator tok_iter = tokens.begin(); tok_iter != tokens.end(); ++tok_iter) {

            // vmm id
            if(parse_state == state_vmm_id) {
                vmm_id_str = boost::trim_copy(*tok_iter);
                parse_state = state_vmm_chan;
            }
            // vmm channel
            else if(parse_state == state_vmm_chan) {
                vmm_chan_str = boost::trim_copy(*tok_iter);
                parse_state = state_feb_chan;
            }
            // feb channel
            else if(parse_state == state_feb_chan) {
                feb_chan_str = boost::trim_copy(*tok_iter);
            }
        } // tok_iter
        if(parse_state != state_feb_chan) {
            ok = false;
            stringstream sx;
            sx << "Connector::fillElxMap    ERROR reading map (" << m_elx_map_name << ") at"
                << " line number " << line_counter;
            cout << sx.str() << endl;
        }

        if(ok) {
            m_elx_map.push_back( std::make_tuple( stoi(vmm_id_str), stoi(vmm_chan_str), stoi(feb_chan_str) ) );
        }
    } // while

    mapfile.close();
    sort(m_elx_map.begin(), m_elx_map.end(), byFebChan);

    //for(auto& elx : m_elx_map) {
    //    cout << std::get<0>(elx) << "  " << std::get<1>(elx) << "  " << std::get<2>(elx) << endl;
    //}

    return ok;
}
// ------------------------------------------------------------------------ //
bool byFebChanDet(const std::tuple<int, int, int> &lhs,
                    const std::tuple<int, int, int> &rhs)
{
    return std::get<0>(lhs) < std::get<0>(rhs);
}
bool Connector::fillChamberMap()
{
    m_chamber_map.clear();
    bool ok = true;

    std::ifstream mapfile(m_chamber_map_name.c_str());
    std::string line;
    int line_counter = 0;

    while(getline(mapfile, line)) {
        line_counter++;

        boost::trim(line);
        if(boost::starts_with(line, "#") || line.empty()) continue;

        typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
        boost::char_separator<char> sep(", \t");
        tokenizer tokens(line, sep);

        std::string feb_chan_str;
        std::string det_type_str;
        std::string det_strip_str;

        enum { state_feb_chan, state_det_type, state_det_strip } parse_state;

        parse_state = state_feb_chan;

        for(tokenizer::iterator tok_iter = tokens.begin(); tok_iter != tokens.end(); ++tok_iter) {

            // feb channel
            if(parse_state == state_feb_chan) {
                feb_chan_str = boost::trim_copy(*tok_iter);
                parse_state = state_det_type;
            }
            // det type
            else if(parse_state == state_det_type) {
                det_type_str = boost::trim_copy(*tok_iter);
                parse_state = state_det_strip;
            }
            // detector strip/element
            else if(parse_state == state_det_strip) {
                det_strip_str = boost::trim_copy(*tok_iter);
            }
        } // tok_iter
        if(parse_state != state_det_strip) {
            ok = false;
            stringstream sx;
            sx << "Connector::fillChamberMap    ERROR reading map (" << m_chamber_map_name << ") at"
                << " line number " << line_counter;
            cout << sx.str() << endl;
        }

        if(ok) {
            m_chamber_map.push_back( std::make_tuple( stoi(feb_chan_str), stoi(det_type_str), stoi(det_strip_str) ) );
        }
    } // while

    mapfile.close();
    sort(m_chamber_map.begin(), m_chamber_map.end(), byFebChanDet);

    //for(auto& strip : m_chamber_map) {
    //    cout << std::get<0>(strip) << "  " << std::get<1>(strip) << "  " << std::get<2>(strip) << endl;
    //}

    return ok;

}
// ------------------------------------------------------------------------ //
bool Connector::hasBoardId(int id)
{
    bool hasit = false;
    if(boardLoaded()) {
        if(board().id() == id) hasit = true;
    }
    else {
        cout << "Connector::hasBoardId    No board has been loaded, returning false" << endl;
        hasit = false;
    }
    return hasit;
}
