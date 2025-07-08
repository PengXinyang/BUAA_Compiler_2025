//
// Created by PengXinyang on 24-12-5.
//

#include "../../../../include/structure/text/MipsInstruction/MTypeInstruction.h"

#include "generate/MipsContent.h"

MTypeInstruction::MTypeInstruction(const string &operation_name, const vector<Register*>& registers) {
    this->operation_name = operation_name;
    if(registers.size()==2) {
        rs = registers[0];
        rt = registers[1];
    }else {
        rd = registers[0];
    }
    MipsContent::addTextSegment(this);
}

string MTypeInstruction::toMips() {
    return (!rd) ? operation_name + " " + rs->toMips() + " " + rt->toMips() : operation_name + " " + rd->toMips();
}
