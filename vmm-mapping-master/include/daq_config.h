#ifndef DAQ_CONFIG_H
#define DAQ_CONFIG_H

/////////////////////////////////////////////////
//
// daq_config
//
// Interface between the map handler tool 
// (c.f. map_handler.h) and the detector setup
// (c.f. det_config.h)
//
// daniel.joseph.antrim@cern.ch
// University of California, Irvine
// September 2016
//
/////////////////////////////////////////////////

//std/stl
#include <vector>
#include <string>
#include <unordered_map>

//nsw
class DetectorConfig;

namespace nsw_map {
    // { BOARD IP : BOARD ID }
    typedef std::unordered_map<std::string, int> boardIpToIdMap;
}; // namespace

class DaqConfiguration {

    public :
        DaqConfiguration();
        virtual ~DaqConfiguration(){};

        void setDebug(bool doit) { m_dbg = doit; }
        bool dbg() { return m_dbg; }

        bool loadDaqXml(std::string filename);
        std::string mapDir() { return m_map_dir; }
        std::string daqConfigFile() { return m_daq_config_file; }

        bool loadDetectorSetup();
        bool checkSetup();
        bool checkLoadedDetectors();
        bool checkLoadedModules();
        bool checkLoadedLayers();
        bool checkLoadedConnectors();
        bool checkLoadedBoards();

        bool loadMaps();

        DetectorConfig& detConfig() { return *m_detConfig; }
        nsw_map::boardIpToIdMap& boardMap() { return m_boardMap; }

    private :
        bool m_dbg;
        std::string m_map_dir;
        std::string m_daq_config_file;

        DetectorConfig* m_detConfig;

        // map of loaded boards
        nsw_map::boardIpToIdMap m_boardMap;


}; // class

#endif
