//nsw
#include "board.h"

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
// Board
// ------------------------------------------------------------------------ //
//////////////////////////////////////////////////////////////////////////////
Board::Board() :
    m_dbg(false),
    m_id(-1),
    m_ip(""),
    m_type(""),
    m_name(""),
    m_chip_type(""),
    n_chip_count(-1)
{
}
// ------------------------------------------------------------------------ //
bool Board::load(const boost::property_tree::ptree::value_type pt)
{
    bool ok = true;
    using boost::property_tree::ptree;
    using namespace boost;

    try {

        BOOST_FOREACH(const ptree::value_type &v, pt.second) {

            /////////////////////////////////////
            // unique ID
            /////////////////////////////////////
            if(v.first=="id") {
                stringstream id_;
                id_ << v.second.data();
                string idstr = id_.str();
                trim(idstr);
                if(idstr=="") {
                    cout << "Board::load    ERROR Board id node is \"\"" << endl;
                    ok = false;
                    break;
                }
                m_id = std::stoi(idstr);
            }
            /////////////////////////////////////
            // unique IP
            /////////////////////////////////////
            else if(v.first=="ip") {
                m_ip = v.second.data();
                trim(m_ip);
            }
            /////////////////////////////////////
            // board type (MMFE8, sFEB, pFEB)
            /////////////////////////////////////
            else if(v.first=="type") {
                m_type = v.second.data();
                trim(m_type);
            }
            /////////////////////////////////////
            // type of chip on this board
            /////////////////////////////////////
            else if(v.first=="chip_type") {
                m_chip_type = v.second.data();
                trim(m_chip_type);
            }
            /////////////////////////////////////
            // number of chips on this board
            /////////////////////////////////////
            else if(v.first=="chip_count") {
                stringstream count;
                count << v.second.data();
                string countstr = count.str();
                trim(countstr);
                if(countstr=="") {
                    cout << "Board::load    ERROR chip count is \"\"" << endl;
                } 
                n_chip_count = std::stoi(countstr);
            }

        } //foreach

    } // try
    catch(std::exception &e) {
        cout << "Board::load    ERROR " << e.what() << endl;
        ok = false;
    }

    
    /////////////////////////////////////////////////////
    // set full name of this board
    stringstream fullname;
    //fullname << "FE." << (id()>9 ? "" : "0") << id() << "." << type();
    fullname << type() << "." << (id()>9 ? "" : "0") << id();
    m_name = fullname.str();

    /////////////////////////////////////////////////////
    // now load the chips
    for(int i = 0; i < n_chip_count; i++) {
        Chip tmpChip;
        if(!tmpChip.load(m_chip_type, i, m_id)) {
            ok = false;
            cout << "Board::load    ERROR loading Chip # " << i << " at Board # " << m_id << endl;
            break;
        }
        if(ok) {
            m_chips.push_back(tmpChip);
        }
    }
    if(!((int)m_chips.size()==n_chip_count)) {
        cout << "Board::load    ERROR Expect " << n_chip_count << " chips for Board # " << m_id
             << " but only " << m_chips.size() << " have been loaded" << endl;
        ok = false;
    }


    return ok;
}
// ------------------------------------------------------------------------ //
string Board::chipNameFromId(int id)
{
    // start at 0
    string out = "";
    if(id > (nChips()-1)) {
        cout << "Board::chipNameFromId    Invalid chip id (" << id << ")" << endl;
        return out;
    }
    if(nChips()>0) {
        return chips().at(id).name();
    }
    return out;
}
