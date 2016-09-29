//nsw
#include "element.h"
#include "element_type.h"

//////////////////////////////////////////////////////////////////////////////
// ------------------------------------------------------------------------ //
// Element
// ------------------------------------------------------------------------ //
//////////////////////////////////////////////////////////////////////////////
Element::Element() :
    m_id(-1),
    m_board_ip(""),
    m_board_id(-1),
    m_board_name(""),
    m_board_type(""),
    m_element_type(-1),
    m_element_type_str(""),
    m_detector_id(-1),
    m_module_id(-1),
    m_layer_id(-1),
    m_connector_id(-1),
    m_vmm_id(-1),
    m_vmm_chan(-1),
    m_feb_chan(-1),
    m_element_number(-1)
    
{
}
// ------------------------------------------------------------------------ //
void Element::setElementType(int type)
{
    m_element_type = type;
    nsw_map::ElementType element_type = nsw_map::ElementTypeFromInt(type);
    m_element_type_str = nsw_map::ElementType2Str(element_type);
}
