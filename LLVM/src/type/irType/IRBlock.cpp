//
// Created by PengXinyang on 24-11-21.
//

#include "../../../include/type/irType/IRBlock.h"

bool IRBlock::isBlock() {
    return true;
}
string IRBlock::toLLVM() {
    return "i32";
}
