//
// Created by PengXinyang on 24-12-4.
//

#include "../../include/structure/Segment.h"

#include <sstream>

#include "instanceof.h"
#include "structure/text/MipsBlock.h"

void Segment::addMipsSegment(MipsStructure* newMipsSegment) {
    MipsSegments.push_back(newMipsSegment);
}

string Segment::toMips() {
    ostringstream oss;
    int num = static_cast<int>(MipsSegments.size());
    for(int i = 0; i < num; i++) {
        if(i+1<num && instanceof<MipsBlock>(MipsSegments[i+1])) {
            oss << MipsSegments[i]->toMips()<<"\n";
        }
        else oss << MipsSegments[i]->toMips()<<"\n\t";
    }
    return oss.str();
}
