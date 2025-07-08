//
// Created by PengXinyang on 24-12-5.
//

#include "../../../../include/structure/text/PseudoInstruction/LaInstruction.h"

#include "generate/MipsContent.h"

LaInstruction::LaInstruction(Register *rd, const string &label) {
    this->rd = rd;
    this->label = label;
    MipsContent::addTextSegment(this);
}
string LaInstruction::toMips() {
    return "la " + rd->toMips() + ", " + label;
}
