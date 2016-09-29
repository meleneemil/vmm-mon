#ifndef CHIP_H
#define CHIP_H

/////////////////////////////////////////////////////
//
// chip
//
// Container class for the readout chip
//
// daniel.joseph.antrim@cern.ch
// University of California, Irvine
// September 2016
//
/////////////////////////////////////////////////////

//std/stl
#include <string>

class Chip {

    public :
        Chip();
        virtual ~Chip(){};

        void setDebug(bool doit) { m_dbg = doit; }
        bool dbg() { return m_dbg; }

        bool load(std::string chip_type, int chip_id, int parent_id);

        std::string type() { return m_type; }
        int id() { return m_id; }
        int parentBoardId() { return m_parent_board_id; }
        std::string name() { return m_name; }

    private :
        bool m_dbg;
        std::string m_type;
        int m_id;
        int m_parent_board_id;
        std::string m_name;

}; // class


#endif
