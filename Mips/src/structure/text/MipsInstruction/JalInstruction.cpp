//
// Created by PengXinyang on 24-12-4.
//

#include "../../../../include/structure/text/MipsInstruction/JalInstruction.h"

#include "generate/MipsContent.h"

JalInstruction::JalInstruction(const string &block_name) {
    this->block_name = block_name;
    MipsContent::addTextSegment(this);
}

string JalInstruction::toMips() {
    return "jal " + block_name;
}