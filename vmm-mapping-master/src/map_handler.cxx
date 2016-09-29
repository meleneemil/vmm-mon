//nsw
#include "map_handler.h"
#include "daq_config.h"
#include "det_config.h"
#include "element_type.h"

//std/stl
#include <iostream>
using namespace std;

//boost
#include <boost/tokenizer.hpp>


//////////////////////////////////////////////////////////////////////////////
// ------------------------------------------------------------------------ //
//  MapHandler
// ------------------------------------------------------------------------ //
//////////////////////////////////////////////////////////////////////////////
MapHandler::MapHandler() :
    m_dbg(false),
    m_initialized(false),
    m_map_loaded(false),
    m_daqConfig(0)
{
}
// ------------------------------------------------------------------------ //
bool MapHandler::loadDaqConfiguration(string filename)
{
    if(m_daqConfig) delete m_daqConfig;

    m_daqConfig = new DaqConfiguration();

    bool ok = m_daqConfig->loadDaqXml(filename);
    if(ok) {
        cout << "MapHandler::loadDaqConfiguration    DAQ XML loaded: " << filename << endl;
    }
    else {
        cout << "MapHandler::loadDaqConfiguration    Unable to load DAQ XML: " << filename << endl;
        return false;
    }
    ok = m_daqConfig->loadDetectorSetup();
    if(ok) {
        cout << "MapHandler::loadDaqConfiguration    Detector setup loaded successfully" << endl;
    }
    else {
        cout << "MapHandler::loadDaqConfiguration    Unable to load detector setup" << endl;
        return false;
    }

    m_initialized = ok;

    if(!ok) delete m_daqConfig;
    return ok;

}
// ------------------------------------------------------------------------ //
bool MapHandler::buildMapping()
{
    bool ok = true;

    if(!m_initialized) {
        cout << "MapHandler::buildMapping    Detector setup has not yet been loaded (c.f. MapHandler::loadDaqConfiguration). Cannot build mapping!" << endl;
        return false;
    }

    int n_element_loaded = 0;
    // loop over boards
    nsw_map::febIdToChannelMap id_map;

    for(auto& boardMap : m_daqConfig->boardMap()) {
        int boardId = boardMap.second;
        string boardIp = boardMap.first;

        nsw_map::febChanToElementMap element_map;

        for(auto& detector : m_daqConfig->detConfig().detectors()) {
            if(!detector.hasBoardId(boardId)) continue;

            for(auto& module : detector.modules()) {
                if(!module.hasBoardId(boardId)) continue;

                for(auto& layer : module.layers()) {
                    if(!layer.hasBoardId(boardId)) continue;

                    for(auto& connector : layer.connectors()) {
                        if(!connector.hasBoardId(boardId)) continue;


                        // fill the contents for building up the monitoring tree
                        nsw_map::boardContentMap tmpMap;
                        vector<string> vmm_names;
                        for(auto chip : connector.board().chips()) {
                            vmm_names.push_back(chip.name());
                        }
                        tmpMap[connector.board().name()] = vmm_names;
                        m_board_contents.push_back(tmpMap);

                        // loop over VMM
                        for(auto& vmm : connector.elxMap()) {
                            int vmm_id = std::get<0>(vmm);
                            int vmm_chan = std::get<1>(vmm);
                            int feb_chan = std::get<2>(vmm);

                            // now get strip for this (VMM id, VMM chan) combo
                            for(auto& element : connector.chamberMap()) {
                                if(!(feb_chan==std::get<0>(element))) continue;

                                int elem_type = std::get<1>(element);
                                int element_number = std::get<2>(element);

                                nsw_map::ElementType element_type = nsw_map::ElementTypeFromInt(elem_type);
                                if(element_type == nsw_map::ElementType::ElementTypeInvalid) {
                                    cout << "MapHandler::buildMapping    ERROR Invalid element type ("
                                         << elem_type << ") found "
                                         << "in mapping for (board id, vmm id, vmm chan, feb chan) = ("
                                         << boardId << ", " << vmm_id << ", " << vmm_chan << ", " << feb_chan
                                         << ")" << endl;
                                    cout << "MapHandler::buildMapping    ERROR >>> Map building aborted!" << endl;
                                    m_map_loaded = false;
                                    return false;
                                }
                                string element_type_str = nsw_map::ElementType2Str(element_type);

                                /////////////////////////////////////////////////
                                // build up the Element object for this element
                                /////////////////////////////////////////////////
                                Element elemObj;

                                elemObj.init(n_element_loaded);

                                elemObj.setBoardIp(boardIp);
                                elemObj.setBoardId(boardId);
                                elemObj.setBoardName(connector.board().name());
                                elemObj.setBoardType(connector.board().type());
                                elemObj.setChipName(connector.board().chipNameFromId(vmm_id));
                                elemObj.setElementType(elem_type);
                                elemObj.setDetectorId(detector.id());
                                elemObj.setModuleId(module.id());
                                elemObj.setLayerId(layer.id());
                                elemObj.setConnectorId(connector.id());

                                elemObj.setVMMId(vmm_id);
                                elemObj.setVMMChan(vmm_chan);
                                elemObj.setFEBChan(feb_chan);
                                elemObj.setElementNumber(element_number);

                                element_map[feb_chan] = elemObj;

                                n_element_loaded++;
                            } // element


                        } // vmm

                    } // connector
                } // layer
            } // module
        } // detector

        id_map[boardId] = element_map;

    } // board

    // this is the final map
    m_daq_map = id_map;

    //cout << m_daq_map[0][166].vmmId() << "  " << m_daq_map[0][166].vmmChan() << "  " << m_daq_map[0][166].elementNumber() << "  " << m_daq_map[0][166].boardType() << "  " << m_daq_map[0][166].chipName() << "  " << m_daq_map[0][166].elementType() << "  " << m_daq_map[0][166].elementTypeStr() << endl;

    // now sort the board_contents by board Id
    sortBoardContents(); 



    m_map_loaded = ok;

    return ok;
}
// ------------------------------------------------------------------------ //
struct boardIdSmaller {
    bool operator()(std::string a, std::string b) { return (MapHandler::getBoardIdFromName(a) < MapHandler::getBoardIdFromName(b)); }
} byBoardId;
void MapHandler::sortBoardContents()
{
    vector<nsw_map::boardContentMap> tmpMap_vector;

    vector<string> board_names;
    for(auto boardidmap : boardContents()) {
        for(auto boardid : boardidmap) {
            board_names.push_back(boardid.first);
        }
    }
    //cout << "***** BEFORE ***** " << endl;
    //for(auto boardname : board_names) {
    //    cout << boardname << endl;
    //}
    
    std::sort(board_names.begin(), board_names.end(), byBoardId);
    //cout << "***** AFTER ***** " << endl;
    //for(auto boardname : board_names) {
    //    cout << boardname << endl;
    //}

    // now fill the contents sorted by board ID smaller to larger
    for(auto boardname : board_names) {
        for(auto boardidmap : boardContents()) {
            for(auto boardid : boardidmap) {
                if(boardid.first==boardname) {
                    nsw_map::boardContentMap tmpMap;
                    vector<string> vmm_names;
                    for(auto vmmname : boardid.second) {
                        vmm_names.push_back(vmmname);
                    }
                    tmpMap[boardname] = vmm_names; 
                    tmpMap_vector.push_back(tmpMap);
                } // at the board we are looking for
            } // boardid
        } // boardidmap
    } // boardname

    // clear the old one
    m_board_contents.clear();
    // assign the new, sorted one
    m_board_contents = tmpMap_vector;
}
// ------------------------------------------------------------------------ //
int MapHandler::getBoardIdFromName(string boardname)
{
    // this method expects that the board naming convention is:
    // <type>.XX
    // where XX is the board ID and type is, e.g. 'MMFE8'

    typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
    boost::char_separator<char> sep(".");
    tokenizer tokens(boardname, sep);
    int which = 0;
    int out = -1;
    try{
        for(auto tok : tokens) {
            if(which==1) out = stoi(tok);
            which++;
        }
    } // try
    catch(std::exception& e) {
        cout << "MapHandler::getBoardIdFromName    Exception caught in extracting board ID from board with"
             << " with name " << boardname << " (return -1)" << endl;
        out = -1;
    }
    return out;
    
}
// ------------------------------------------------------------------------ //
int MapHandler::numberOfBoards()
{
    int out = 0;
    if(!m_initialized) {
        cout << "MapHandler::numberOfBoards    WARNING Detector setup has not yet been loaded (c.f. MapHandler::loadDaqConfiguration). Cannot return number of boards!" << endl;
    }
    else {
        for(auto& boardMap : m_daqConfig->boardMap()) {
            out++;
        } // boardMap
    }
    return out;
}
// ------------------------------------------------------------------------ //
string MapHandler::firstIP()
{
    string out = "0.0.0.0";
    if(!m_initialized) {
        cout << "MapHandler::firstIP    WARNING Detector setup has not yet been loaded (c.f. MapHandler::loadDaqConfiguration). Cannot return requested IP!" << endl;
        return out;
    }

    for(auto& boardMap : m_daqConfig->boardMap()) {
        int boardId = boardMap.second;
        string boardIp = boardMap.first;
        if(boardId==0) {
            out = boardIp;
            break;
        }
    }
    return out;
}
// ------------------------------------------------------------------------ //
bool MapHandler::isValidBoardId(int boardId_)
{
    bool valid = false;
    for(auto& boardMap : m_daqConfig->boardMap()) {
        if(boardId_ == boardMap.second) {
            valid = true;
            break;
        }
    }
    return valid;
}
// ------------------------------------------------------------------------ //
int MapHandler::boardIDfromIP(string ip_string)
{
    int out = -1;
    if(!mapLoaded()) {
        cout << "MapHandler::boardIDfromIP    WARNING Map is not yet loaded. Cannot return requested ID" << endl;
        return out;
    }

    for(auto& boardMap : m_daqConfig->boardMap()) {
        int boardId = boardMap.second;
        string boardIp = boardMap.first;
        if(ip_string == boardIp) {
            out = boardId;
            break;
        }
    } // boardMap
    return out;
}
// ------------------------------------------------------------------------ //
int MapHandler::boardChannel(int boardId_, int vmmId_, int vmmChan_)
{

    if(mapLoaded()) {
        for(auto& boardMap : m_daqConfig->boardMap()) {
            int boardId = boardMap.second;
            string boardIp = boardMap.first;
            if( !(boardId==boardId_) ) continue;
            for(auto& detector : m_daqConfig->detConfig().detectors()) {
                if(!detector.hasBoardId(boardId)) continue;

                for(auto& module : detector.modules()) {
                    if(!module.hasBoardId(boardId)) continue;

                    for(auto& layer : module.layers()) {
                        if(!layer.hasBoardId(boardId)) continue;

                        for(auto& connector : layer.connectors()) {
                            if(!connector.hasBoardId(boardId)) continue;

                            for(auto& vmm : connector.elxMap()) {
                                int vmm_id = std::get<0>(vmm);
                                int vmm_chan = std::get<1>(vmm);
                                int feb_chan = std::get<2>(vmm);

                                if( (vmm_id == vmmId_) && (vmm_chan == vmmChan_))
                                    return feb_chan;
                            } // vmm
                        } // connector
                    } // layer
                } // module
            } // detector

        } // boardMap
    } // mapLoaded

    cout << "MapHandler::boardChannel    WARNING Unable to find for (vmm ID, vmm Channel, board Id) = ("
         << vmmId_ << ", " << vmmChan_ << ", " << boardId_ << ") the associated board channel (return -1)!"
         << " Maps loaded: " << (mapLoaded() ? "YES" : "NO") << endl;
    return -1;
}
// ------------------------------------------------------------------------ //
int MapHandler::elementNumber(int boardId_, int vmmId_, int vmmChan_)
{
    if(mapLoaded()) {

        bool is_valid_board = isValidBoardId(boardId_);

        if(is_valid_board) {
            int feb_chan = boardChannel(boardId_, vmmId_, vmmChan_);
            if(!(feb_chan<0)) {
                return m_daq_map[boardId_][feb_chan].elementNumber();
            }
        }
        else {
            cout << "MapHandler::elementNumber    WARNING Provided board ID (" << boardId_ << ") is not valid "
                 << "(it is not in the list of loaded boards) (return -1)" << endl;
            return -1;
        }
    } // mapLoaded
    else {
        cout << "MapHandler::elementNumber    WARNING Mapping is not loaded. Cannot provide detector element "
             << "number associated with (vmm ID, vmm Channel, board ID) = ("
             << vmmId_ << ", " << vmmChan_ << ", " << boardId_ << ") (return -1)" << endl;
        return -1;
    }
    return -1;
}
// ------------------------------------------------------------------------ //
string MapHandler::chipName(int boardId_, int vmmId_)
{
    if(mapLoaded()) {
        bool is_valid_board = isValidBoardId(boardId_);
        if(is_valid_board) {

            for(auto& boardMap : m_daqConfig->boardMap()) {
                int boardId = boardMap.second;
                if( !(boardId==boardId_) ) continue;
                for(auto& detector : m_daqConfig->detConfig().detectors()) {
                    if(!detector.hasBoardId(boardId)) continue;

                    for(auto& module : detector.modules()) {
                        if(!module.hasBoardId(boardId)) continue;

                        for(auto& layer : module.layers()) {
                            if(!layer.hasBoardId(boardId)) continue;

                            for(auto& connector : layer.connectors()) {
                                if(!connector.hasBoardId(boardId)) continue;

                                for(auto& chip : connector.board().chips()) {
                                    if(chip.id() == vmmId_) {
                                        return chip.name();
                                    }
                                } // chip
                            } // connector
                        } // layer
                    } // module
                } // detector
            } // boardMap
        } // valid board Id

        else {
            cout << "MapHandler::chipName    WARNING Provided board ID (" << boardId_ << ") is not valid "
                 << "(it is not in the list of loaded boards) (return \"\")" << endl;
            return "";
        }
    } // mapLoaded
    else {
        cout << "MapHandler::chipName    WARNING Mapping is not loaded. Cannot provide chip name "
             << "associated with (vmm ID, board ID) = ("
             << vmmId_ << ", " << boardId_ << ") (return \"\")" << endl;
        return "";
    }
    return "";
}
