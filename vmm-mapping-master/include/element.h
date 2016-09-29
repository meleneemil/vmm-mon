#ifndef ELEMENT_H
#define ELEMENT_H

//////////////////////////////////////////
//
// element
//
// Container for information about the
// detector element being read out
//
// daniel.joseph.antrim@cern.ch
// University of California, Irvine
// September 2016
//
//////////////////////////////////////////

#include <string>

class Element {

    public :
        Element();
        virtual ~Element(){};

        void init(int i) { m_id = i; }

        int id() { return m_id; }


        // setters

        void setBoardIp(std::string ip)             { m_board_ip = ip; }
        void setBoardId(int id)                     { m_board_id = id; }
        void setBoardName(std::string name)         { m_board_name = name; }
        void setBoardType(std::string type)         { m_board_type = type; }
        void setChipName(std::string name)          { m_chip_name = name; }

        void setElementType(int type);

        void setDetectorId(int id)                  { m_detector_id = id; }
        void setModuleId(int id)                    { m_module_id = id; }
        void setLayerId(int id)                     { m_layer_id = id; }
        void setConnectorId(int id)                 { m_connector_id = id; }

        void setVMMId(int id)                       { m_vmm_id = id; }
        void setVMMChan(int chan)                   { m_vmm_chan = chan; }
        void setFEBChan(int chan)                   { m_feb_chan = chan; }
        void setElementNumber(int no)               { m_element_number = no; }

        // getters
        std::string boardIp()                       { return m_board_ip; }
        int boardId()                               { return m_board_id; }
        std::string boardName()                     { return m_board_name; }
        std::string boardType()                     { return m_board_type; }
        std::string chipName()                      { return m_chip_name; }

        int elementType()                           { return m_element_type; }
        std::string elementTypeStr()                { return m_element_type_str; }

        int detectorId()                            { return m_detector_id; }
        int moduleId()                              { return m_module_id; }
        int layerId()                               { return m_layer_id; }
        int connectorId()                           { return m_connector_id; }

        int vmmId()                                 { return m_vmm_id; }
        int vmmChan()                               { return m_vmm_chan; }
        int febChan()                               { return m_feb_chan; }
        int elementNumber()                         { return m_element_number; }

    private :
        int m_id; // unique id for this element

        std::string m_board_ip;             // IP of containing FEB that reads this element out
        int m_board_id;                     // ID of containing FEB that reads this element out
        std::string m_board_name;           // name of containing FEB that reads this element out 
        std::string m_board_type;           // type of containing FEB that reads this element out
        std::string m_chip_name;            // name of VMM that reads this element out
        

        int m_element_type;                 // type of this element (c.f. element_type.h)
        std::string m_element_type_str;     // type of this element (c.f. elemnet_type.h)

        int m_detector_id;                  // ID of containing detector
        int m_module_id;                    // ID of containing module
        int m_layer_id;                     // ID of containing layer
        int m_connector_id;                 // ID of connector that reads this element out

        int m_vmm_id;                       // ID of VMM that reads this element out
        int m_vmm_chan;                     // Channel number of VMM that reads this element out
        int m_feb_chan;                     // Channel number of FEB that reads this element out
        int m_element_number;               // number of this element 

}; // class

#endif
