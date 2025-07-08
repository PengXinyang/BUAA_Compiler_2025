//
// Created by PengXinyang on 24-12-5.
//

#include "../../../../include/structure/text/MipsInstruction/RTypeInstruction.h"

#include "generate/MipsContent.h"

RTypeInstruction::RTypeInstruction(const string &operation_name, const vector<Register *> &registers) {
    this->operation_name = operation_name;
    if(registers.size() == 3) {
        rd = registers[0];
        rs = registers[1];
        rt = registers[2];
    }else {
        //jr指令，只用一个寄存器
        rd = registers[0];
    }
    MipsContent::addTextSegment(this);
}

string RTypeInstruction::toMips() {
    if(!rs || !rt) {
        //rs和rt都是nullptr
        return operation_name + " " + rd->toMips();
    }
    return operation_name + " " + rd->toMips() +", "+ rs->toMips() +", "+ rt->toMips();
}
