//
// Created by PengXinyang on 24-11-21.
//

#include "../../../include/type/irType/IRInt.h"

bool IRInt::isInt() {
    return true;
}
string IRInt::toLLVM() {
    return "i32";
}
