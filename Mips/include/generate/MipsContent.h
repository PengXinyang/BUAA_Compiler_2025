//
// Created by PengXinyang on 24-12-4.
//

#ifndef MIPSDATA_H
#define MIPSDATA_H
#include "structure/DataSegment.h"
#include "structure/Segment.h"
#include "structure/TextSegment.h"


class MipsContent {
private:
    static TextSegment* textSegment;//text段
    static DataSegment* dataSegment;//data段
public:
    MipsContent();
    static void addTextSegment(MipsStructure* mips_structure);
    static void addDataSegment(MipsStructure* mips_structure);
    string toMips();
};



#endif //MIPSDATA_H
