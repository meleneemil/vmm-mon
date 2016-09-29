//nsw
#include "element_type.h"

using std::string;

//////////////////////////////////////////////////////////////////////////////
// ------------------------------------------------------------------------ //
// ElementType
// ------------------------------------------------------------------------ //
//////////////////////////////////////////////////////////////////////////////
namespace nsw_map
{
    std::string ElementType2Str(const ElementType &e)
    {
        string o = "Unknown";
        switch(e) {
            case ElementType::MMstrip               : o = "MM-strip"           ; break;
            case ElementType::TGCstrip              : o = "TGC-strip"          ; break;
            case ElementType::TGCwire               : o = "TGC-wire"           ; break;
            case ElementType::TGCpad                : o = "TGC-pad"            ; break;
            case ElementType::ElementTypeInvalid    : o = "ElementTypeInvalid" ; break;
        } // switch
        return o;
    }

    ElementType ElementTypeFromInt(int i)
    {
        ElementType el = ElementTypeInvalid;
        switch(i) {
            case 0           : el = MMstrip;         break;
            case 1           : el = TGCstrip;        break;
            case 2           : el = TGCwire;         break;
            case 3           : el = TGCpad;          break;
        } // switch
        return el;
    }

} //namespace
