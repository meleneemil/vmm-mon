#ifndef ELEMENT_TYPE_H
#define ELEMENT_TYPE_H

#include <string>

namespace nsw_map
{
    enum ElementType {
        MMstrip=0
        ,TGCstrip
        ,TGCwire
        ,TGCpad
        ,ElementTypeInvalid
    };

    // human readable names
    std::string ElementType2Str(const ElementType &e);

    ElementType ElementTypeFromInt(int i);


} // namespace nsw

#endif
