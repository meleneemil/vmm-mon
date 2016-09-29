#ifndef DET_CONFIG_H
#define DET_CONFIG_H

/////////////////////////////////////////////
//
// det_config
//
// Handler class for holding the detecor
// configuration objects built up from the
// DAQ configuration xml
//
//  - holds module, layer, etc... instances
//      (an abstraction of the experimental
//      setup)
//
// daniel.joseph.antrim@cern.ch
// University of California, Irvine
// September 2016
//
//////////////////////////////////////////////

#include <vector>
#include <string>

//nsw
#include "detector.h"

class DetectorConfig {

    public :
        DetectorConfig();
        virtual ~DetectorConfig(){};

        void setDebug(bool doit) { m_dbg = doit; }
        bool dbg() { return m_dbg; }

        void setMapDir(std::string mapdir) { m_map_dir = mapdir; }

        // load the detector configuration based on the input xml
        bool loadDetectorSetup(std::string filename);

        std::string name() { return m_name; }

        std::vector<Detector>& detectors() { return m_detectors; };
        int nDetectors() { return (int)m_detectors.size(); }

    private :
        bool m_dbg;
        std::string m_map_dir;

        std::string m_name;

        // container for loaded detectors (e.g. sectors)
        std::vector<Detector> m_detectors;

}; // class


#endif
