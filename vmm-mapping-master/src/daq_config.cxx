//nsw
#include "daq_config.h"
#include "det_config.h"

//std/stl
#include <iostream>
#include <fstream>
#include <algorithm>
using namespace std;

//boost
#include <boost/filesystem.hpp>


//////////////////////////////////////////////////////////////////////////////
// ------------------------------------------------------------------------ //
//  DaqConfiguration
// ------------------------------------------------------------------------ //
//////////////////////////////////////////////////////////////////////////////
DaqConfiguration::DaqConfiguration() :
    m_dbg(false),
    m_map_dir(""),
    m_daq_config_file(""),
    m_detConfig(0)
{
    // clear maps
    m_boardMap.clear();
}
// ------------------------------------------------------------------------ //
bool DaqConfiguration::loadDaqXml(string filename)
{

    string full_filename = filename;
    bool exists = std::ifstream(full_filename).good();

    if(!exists) {
        cout << "DaqConfiguration::loadDaqXml    File (" << full_filename << ") not found" << endl;
        return false;
    }

    m_daq_config_file = full_filename;

    boost::filesystem::path p_config(full_filename);
    boost::filesystem::path dir = p_config.parent_path();

    m_map_dir = dir.string();

    cout << "Successfully found DAQ configuration file" << endl;
    cout << "    > DAQ config      : " << full_filename << endl;
    cout << "    > map directory   : " << m_map_dir << endl;

   // if(detectorConfigFile()!="") {
   //     exists = ifstream(full_det_config_name).good();
   // }
   // if(!exists) {
   //     cout << "****************************************************************************" << endl;
   //     cout << "DaqConfiguration::loadDaqXml    ERROR detector config file does not exists: "
   //          << full_det_name << endl;
   //     cout << "****************************************************************************" << endl;
   //     return false;
   // }

   // cout << "Successfully found DAQ configuration files" << endl;
   // cout << "    > detector config : " << full_det_config_name << endl;
   // cout << "    > map directory   : " << m_map_dir << endl;

   // //update filenames to have the full path
   // m_detectorConfigFile = full_det_config_name;

    return true;
}
// ------------------------------------------------------------------------ //
bool DaqConfiguration::loadDetectorSetup()
{
    bool ok = true;
    cout << "DaqConfiguration::loadDetectorSetup" << endl;
    m_detConfig = new DetectorConfig();
    m_detConfig->setMapDir(m_map_dir);

    // read the XML and fill the objects
    ok = m_detConfig->loadDetectorSetup(m_daq_config_file);

    // sanity check(s)
    if(ok)
        ok = checkSetup();

    // load the data from the elx/chamber maps
    if(ok)
        ok = loadMaps();

    return ok;
}
// ------------------------------------------------------------------------ //
bool DaqConfiguration::checkSetup()
{
    /// currently everything needs to be unique.. though the names of children
    /// (e..g. modules) should be repeatable withina given detector but the
    /// identifier should be unique and should have a common 

    cout << "-------------------------------------------------------------- " << endl;
    cout << "Checking loaded setup..." << endl;
    bool ok = true;

    // make sure no detector ID/name is repeated
    cout << "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - " << endl;
    ok = checkLoadedDetectors();
    cout << "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - " << endl;
    // make sure no module ID/name is repeated (within a given detector)
    ok = checkLoadedModules();
    cout << "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - " << endl;
    // make sure no layer ID is repeated (within a given module)
    ok = checkLoadedLayers();
    cout << "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - " << endl;
    // make sure no connector ID is repeated (within a given layer)
    ok = checkLoadedConnectors();
    cout << "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - " << endl;
    // make sure all board IP's and ID's are unique (and fill the board map if so)
    ok = checkLoadedBoards();
    cout << "-------------------------------------------------------------- " << endl;


    return ok;
}
// ------------------------------------------------------------------------ //
bool DaqConfiguration::checkLoadedDetectors()
{
    cout << " > Checking loaded detectors..." << endl;
    bool ok = true;

    #warning implement unique identifiers that inherit parent information
    vector<int> loadedIDs;
    vector<string> loadedNames;

    for(auto& detector : m_detConfig->detectors()) {
        int id_ = detector.id();
        string name_ = detector.name();

        // duplicate id
        if(std::find(loadedIDs.begin(), loadedIDs.end(), id_) != loadedIDs.end()) {
            ok = false;
            cout << "DaqConfiguration::checkLoadedDetectors    ERROR "
                 << "Attempting to load detector ID (detector name: " << detector.name() << ")"
                 << " that has already been registered (ID already registed: " << id_ << ")!" << endl;
        }

        // duplicate name
        if(std::find(loadedNames.begin(), loadedNames.end(), name_) != loadedNames.end()) {
            ok = false;
            cout << "DaqConfiguration::checkLoadedDetectors    ERROR "
                 << "Attempting to load detector with name that is already registed "
                 << " (name: " << detector.name() << ")!" << endl;
        }

        loadedIDs.push_back(id_);
        loadedNames.push_back(name_);

    } // detector

    if(ok) {
        cout << " > Checking detectors: successful!" << endl;
    }
    else {
        cout << " > Checking detectors: failed" << endl;
    }

    return ok;
}
// ------------------------------------------------------------------------ //
bool DaqConfiguration::checkLoadedModules()
{
    cout << " > Checking loaded modules..." << endl;
    bool ok = true;

    vector<int> loadedIDs;
    vector<string> loadedNames;

    for(auto& detector : m_detConfig->detectors()) {

        loadedIDs.clear();
        loadedNames.clear();

        for(auto& module : detector.modules()) {
            int id_ = module.id();
            string name_ = module.name();

            // duplicate ID?
            if(std::find(loadedIDs.begin(), loadedIDs.end(), id_) != loadedIDs.end()) {
                ok = false;
                cout << "DaqConfiguration::checkLoadedModules    ERROR "
                     << "Module with repeated ID (module id: " << module.id()
                     << ", module name: " << module.name() << ") in "
                     << "detector with (detector id: " << detector.id()
                     << ", detector name: " << detector.name() << ")!" << endl;
            }

            // duplicate name?
            if(std::find(loadedNames.begin(), loadedNames.end(), name_) != loadedNames.end()) {
                ok = false;
                cout << "DaqConfiguration::checkLoadedModules    ERROR "
                     << "Module with repeated name (module id: " << module.id()
                     << ", module name: " << module.name() << ") in "
                     << "detector with (detector id: " << detector.id()
                     << ", detector name: " << detector.name() << ")!" << endl;
            }

            loadedIDs.push_back(id_);
            loadedNames.push_back(name_);
        } // module
    } // detector

    if(ok) {
        cout << " > Checking modules: successful!" << endl;
    }
    else {
        cout << " > Checking modules: failed" << endl;
    }

    return ok;

}
// ------------------------------------------------------------------------ //
bool DaqConfiguration::checkLoadedLayers()
{
    cout << " > Checking loaded layers..." << endl;
    bool ok = true;

    vector<int> loadedIDs;
    for(auto& detector : m_detConfig->detectors()) {
        for(auto& module : detector.modules()) {
            loadedIDs.clear();
            for(auto& layer : module.layers()) {
                int id_ = layer.id();

                // duplicate ID?
                if(std::find(loadedIDs.begin(), loadedIDs.end(), id_) != loadedIDs.end()) {
                    ok = false;
                    cout << "DaqConfiguration::checkLoadedLayers    ERROR "
                         << "Repeated layer ID (repeated ID: " << id_ << ") in (detector id, module id) = ("
                         << detector.id() << ", " << module.id() << ")!" << endl; 
                }

                loadedIDs.push_back(id_);
            } // layer
        } // module
    } // detector

    if(ok) {
        cout << " > Checking layers: successful!" << endl;
    }
    else {
        cout << " > Checking layers: failed" << endl;
    }
    return ok;
}
// ------------------------------------------------------------------------ //
bool DaqConfiguration::checkLoadedConnectors()
{
    cout << " > Checking loaded connectors..." << endl;
    bool ok = true;

    #warning should check that connector type is acceptable and expected from module type

    vector<int> loadedIDs;

    for(auto& detector : m_detConfig->detectors()) {
        for(auto& module : detector.modules()) {
            for(auto& layer : module.layers()) {
                loadedIDs.clear();
                for(auto& connector : layer.connectors()) {
                    int id_ = connector.id();

                    // duplicate ID?
                    if(std::find(loadedIDs.begin(), loadedIDs.end(), id_) != loadedIDs.end()) {
                        ok = false;
                        cout << "DaqConfiguration::checkLoadedConnectors    ERROR "
                             << "Repeated connector ID (repeatdd ID: " << id_ << ") "
                             << " in (detector id, module id, layer id) = ("
                             << detector.id() << ", " << module.id() << ", " << layer.id() << ")!" << endl;
                    }

                    loadedIDs.push_back(id_);
                } // connector
            } // layer
        } // module
    } // detector

    if(ok) {
        cout << " > Checking connectors: successful!" << endl;
    }
    else {
        cout << " > Checking connectors: failed" << endl;
    }
    return ok;
}
// ------------------------------------------------------------------------ //
bool DaqConfiguration::checkLoadedBoards()
{
    cout << " > Checking loaded boards..." << endl;

    bool ok = true;

    // build up list of all board ID's and IP's
    vector<string> loadedIPs;
    vector<int> loadedIDs;

    for(auto& detector : m_detConfig->detectors()) {
        for(auto& module : detector.modules()) {
            for(auto& layer : module.layers()) {
                for(auto& connector : layer.connectors()) {
                    if(connector.boardLoaded()) {
                        string ip_ = connector.board().ip();
                        int id_ = connector.board().id();

                        // duplicate IP?
                        if(std::find(loadedIPs.begin(), loadedIPs.end(), ip_) != loadedIPs.end()) {
                            ok = false;
                            cout << "DaqConfiguration::checkLoadedBoards    ERROR "
                                 << "Attempting to load board (name: "
                                 << connector.board().name() << ") with an IP that has already "
                                 << "been registered (" << ip_ << ")!" << endl;
                        }
                        if(std::find(loadedIDs.begin(), loadedIDs.end(), id_) != loadedIDs.end()) {
                            ok = false;
                            cout << "DaqConfiguration::checkLoadedBoards    ERROR "
                                 << "Attempting to load board (name: "
                                 << connector.board().name() << ") with ID that has already "
                                 << "been registered (" << id_ << ")!" << endl;
                        }

                        if(connector.board().type()=="MMFE8" && connector.board().nChips()>8) {
                            ok = false;
                            cout << "DaqConfiguration::checkLoadedBoards    ERROR "
                                 << "Board at (detector id, module id, layer id, connector id) = ("
                                 << detector.id() << ", " << module.id() << ", " << layer.id()
                                 << ", " << connector.id() << ") with ID=" << id_ << " is of type MMFE8 but has "
                                 << "more than 8 chips associated with it (number of chips: "
                                 << connector.board().nChips() << ")!" << endl;
                        }

                        loadedIPs.push_back(ip_);
                        loadedIDs.push_back(id_);
                    } // board is loaded at this connector 
                } // connector
            } // layer
        } // module
    } // detector

    if(ok) {
        for(auto& detector : m_detConfig->detectors()) {
            for(auto& module : detector.modules()) {
                for(auto& layer : module.layers()) {
                    for(auto& connector : layer.connectors()) {
                        if(connector.boardLoaded()) {

                            m_boardMap[connector.board().ip()] = connector.board().id();

                        } // board is loaded at this connector 
                    } // connector
                } // layer
            } // module
        } // detector
    }

    if(m_boardMap.size() != loadedIPs.size()) {
        ok = false;
        cout << "DaqConfiguration::checkLoadedBoards    ERROR Inconsistency between loaded IPs and board map size" << endl;
    }

    if(ok) {
        cout << " > Checking boards: successful!" << endl;
    }
    else {
        cout << " > Checking boards: failed" << endl;
    }

    return ok;
}
// ------------------------------------------------------------------------ //
bool DaqConfiguration::loadMaps()
{
    cout << "-------------------------------------------------------------- " << endl;
    cout << "Loading electronics and detector maps..." << endl;
    bool ok = true;

    for(auto& detector : m_detConfig->detectors()) {
        for(auto& module : detector.modules()) {
            for(auto& layer : module.layers()) {
                for(auto& connector : layer.connectors()) {

                    if(!connector.fillElxMap()) {
                        ok = false;
                    }
                    if(!connector.fillChamberMap()) {
                        ok = false;
                    }
                } // connector
            } // layer
        } // module
    } // detector

    if(ok) {
        cout << " > Loading maps: successful!" << endl;
    }
    else {
        cout << " > Loading maps: failed" << endl;
    }

    cout << "-------------------------------------------------------------- " << endl;
    return ok;
}
