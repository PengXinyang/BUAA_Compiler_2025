//
// Created by PengXinyang on 24-11-21.
//

#include "../../include/type/IRType.h"

#include "../../include/type/irType/IRVoid.h"

bool IRType::isVoid() {
    return false;
}
bool IRType::isArray() {
    return false;
}
bool IRType::isChar() {
    return false;
}
bool IRType::isInt() {
    return false;
}
bool IRType::isBool() {
    return false;
}
bool IRType::isPointer() {
    return false;
}
bool IRType::isBlock() {
    return false;
}

string IRType::toLLVM() {
    return "";
}
