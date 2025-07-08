//
// Created by PengXinyang on 24-11-21.
//

#include "../../../include/type/irType/IRVoid.h"

bool IRVoid::isVoid() {
    return true;
}

string IRVoid::toLLVM() {
    return "void";
}
