//
// Created by PengXinyang on 24-12-5.
//

#include "structure/text/PseudoInstruction/MoveInstruction.h"

#include "generate/MipsContent.h"

MoveInstruction::MoveInstruction(Register *dst, Register *src) {
    this->dst = dst;
    this->src = src;
    MipsContent::addTextSegment(this);
}

string MoveInstruction::toMips() {
    return "move " + this->dst->toMips() +" "+ this->src->toMips();
}
