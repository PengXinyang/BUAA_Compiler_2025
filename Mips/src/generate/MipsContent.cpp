//
// Created by PengXinyang on 24-12-4.
//

#include "../../include/generate/MipsContent.h"

TextSegment* MipsContent::textSegment;
DataSegment* MipsContent::dataSegment;
MipsContent::MipsContent() {
    textSegment = new TextSegment();
    dataSegment = new DataSegment();
}

void MipsContent::addDataSegment(MipsStructure *mips_structure) {
    dataSegment->addMipsSegment(mips_structure);
}

void MipsContent::addTextSegment(MipsStructure *mips_structure) {
    textSegment->addMipsSegment(mips_structure);
}

string MipsContent::toMips() {
    return ".data\n" + dataSegment->toMips() + "\n\n\n.text\n\t" + textSegment->toMips();
}
