//
// Created by PengXinyang on 24-12-5.
//

#include "../../../../include/structure/text/PseudoInstruction/LiInstruction.h"

#include "generate/MipsContent.h"

LiInstruction::LiInstruction(Register *rd, int number) {
    this->rd = rd;
    this->number = number;
    MipsContent::addTextSegment(this);
}
string LiInstruction::toMips() {
    return "li " + rd->toMips() + " " + to_string(number);
}
