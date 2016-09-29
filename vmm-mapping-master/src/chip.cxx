//nsw
#include "chip.h"

//std/stl
#include <iostream>
#include <sstream>
using namespace std;

//////////////////////////////////////////////////////////////////////////////
// ------------------------------------------------------------------------ //
// Chip
// ------------------------------------------------------------------------ //
//////////////////////////////////////////////////////////////////////////////
Chip::Chip() :
    m_dbg(false),
    m_type(""),
    m_id(-1),
    m_parent_board_id(-1),
    m_name("")
{
}
// ------------------------------------------------------------------------ //
bool Chip::load(string type_, int id, int parent_board_id)
{
    bool ok = true;

    m_type = type_;
    m_id = id;
    m_parent_board_id = parent_board_id;

    stringstream name;
    name << type_ << "." << (parent_board_id>9 ? "" : "0") << parent_board_id << "." << id;
    m_name = name.str();

    if(m_type == "") {
        cout << "Chip::load   ERROR provided chip type is \"\"" << endl;
        ok = false;
    }
    if(m_id<0) {
        cout << "Chip::load    ERROR provided chip type is negative (uninitialized)" << endl;
        ok = false;
    }
    if(m_id>=8) {
        cout << "Chip::load    ERROR chip id is >=8 (8 is the absolute max!)" << endl;
        ok = false;
    }
    if(m_parent_board_id<0) {
        cout << "Chip::load    ERROR provided parent board id is negative (uninitialized)" << endl;
        ok = false;
    }

    return ok;
}
