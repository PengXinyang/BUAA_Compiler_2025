//
// Created by PengXinyang on 24-11-28.
//

#include "../../../include/type/irType/IRBool.h"

bool IRBool::isBool() {
    return true;
}

string IRBool::toLLVM() {
    return "i1";
}
