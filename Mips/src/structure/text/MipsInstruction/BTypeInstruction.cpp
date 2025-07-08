//
// Created by PengXinyang on 24-12-5.
//

#include "../../../../include/structure/text/MipsInstruction/BTypeInstruction.h"

#include "generate/MipsContent.h"

BTypeInstruction::BTypeInstruction(const string &operation_name, Register* rs, Register* rt, const string &label) {
    this->operation_name = operation_name;
    this->rs = rs;
    this->rt = rt;
    this->label = label;
    MipsContent::addTextSegment(this);
}

string BTypeInstruction::toMips() {
    if(!rt) {
        return operation_name + " " + rs->toMips() + " " + label;
    }
    return operation_name + " " + rs->toMips() + " " + rt->toMips() + " " + label;
}
