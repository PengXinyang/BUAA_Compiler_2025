//
// Created by PengXinyang on 24-12-4.
//

#include "structure/text/MipsInstruction/JInstruction.h"

#include "generate/MipsContent.h"

JInstruction::JInstruction(const string &block_name) {
    this->block_name = block_name;
    MipsContent::addTextSegment(this);
}

string JInstruction::toMips() {
    return "j " + block_name;
}
