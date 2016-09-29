#ifndef BOARD_H
#define BOARD_H

//////////////////////////////////////////
//
// board
//
// Class for holding information about the
// front end boards (MMFE8, sFEB, pFEB)
//
// - contains access to chips (VMMs) on
//   this board 
//
// daniel.joseph.antrim@cern.ch
// University of California, Irvine
// September 2016
//
//////////////////////////////////////////

//nsw
#include "chip.h"

//std/stl
#include <vector>
#include <string>

//boost
#include <boost/property_tree/ptree.hpp>

class Board {

    public :
        Board();
        virtual ~Board(){};

        void setDebug(bool doit) { m_dbg = doit; }
        bool dbg() { return m_dbg; }

        bool load(const boost::property_tree::ptree::value_type pt);

        int id() { return m_id; }
        std::string ip() { return m_ip; }
        std::string type() { return m_type; }
        std::string name() { return m_name; }
        std::string chipType() { return m_chip_type; }

        std::vector<Chip>& chips() { return m_chips; }
        int nChips() { return (int)m_chips.size(); }

        std::string chipNameFromId(int chipId);

    private :
        bool m_dbg;
        int m_id;
        std::string m_ip;
        std::string m_type;
        std::string m_name;
        std::string m_chip_type;
        int n_chip_count;

        std::vector<Chip> m_chips;

}; // board

#endif
