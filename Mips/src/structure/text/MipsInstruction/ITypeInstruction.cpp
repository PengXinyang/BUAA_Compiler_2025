//
// Created by PengXinyang on 24-12-5.
//

#include "../../../../include/structure/text/MipsInstruction/ITypeInstruction.h"

#include "generate/MipsContent.h"

ITypeInstruction::ITypeInstruction(const string &operation_name, Register *rd, Register *rs, const int number) {
    this->operation_name = operation_name;
    this->rs = rs;
    this->rd = rd;
    this->number = number;
    MipsContent::addTextSegment(this);
}

string ITypeInstruction::toMips() {
    return operation_name + " " + rd->toMips() + " " + rs->toMips() + " " + to_string(number);
}
