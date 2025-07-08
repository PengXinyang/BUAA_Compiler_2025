//
// Created by PengXinyang on 24-11-21.
//

#include "../../../include/type/irType/IRPointer.h"

bool IRPointer::isPointer() {
    return true;
}
IRPointer::IRPointer(IRType* ir_type) {
    irType = ir_type;
}

string IRPointer::toLLVM() {
    return irType->toLLVM()+"*";
}
