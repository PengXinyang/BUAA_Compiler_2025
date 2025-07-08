//
// Created by PengXinyang on 24-12-4.
//

#include "../../../include/structure/text/MipsBlock.h"

#include "generate/MipsContent.h"

MipsBlock::MipsBlock(const string &block_name) {
    this->block_name = block_name;
    MipsContent::addTextSegment(this);
}
string MipsBlock::toMips() {
    return block_name+":";
}
