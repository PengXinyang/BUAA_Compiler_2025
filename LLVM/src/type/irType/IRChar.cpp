//
// Created by PengXinyang on 24-11-21.
//

#include "../../../include/type/irType/IRChar.h"

bool IRChar::isChar() {
    return true;
}
string IRChar::toLLVM() {
    return "i8";
}
