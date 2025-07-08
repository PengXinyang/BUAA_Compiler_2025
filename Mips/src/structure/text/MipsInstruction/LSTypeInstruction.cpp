//
// Created by PengXinyang on 24-12-5.
//

#include "../../../../include/structure/text/MipsInstruction/LSTypeInstruction.h"

#include "generate/MipsContent.h"

LSTypeInstruction::LSTypeInstruction(const string &operation_name, const string &label, Register *rd, Register *base, int offset) {
    this->operation_name = operation_name;
    this->label = label;
    this->rd = rd;
    this->base = base;
    this->offset = offset;
    MipsContent::addTextSegment(this);
}

string LSTypeInstruction::toMips() {
    if(!base) {
        return operation_name + " " + rd->toMips() + " "+ label + " " + to_string(offset);
    }
    return operation_name + " " + rd->toMips() + " " + to_string(offset) + "(" + base->toMips() + ")";
}
