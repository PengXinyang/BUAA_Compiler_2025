//
// Created by PengXinyang on 24-12-7.
//

#include "../../include/structure/DataSegment.h"

#include <sstream>

string DataSegment::toMips() {
    ostringstream oss;
    int num = static_cast<int>(MipsSegments.size());
    for(int i = 0; i < num; i++) {
        oss <<".align 2\n" <<MipsSegments[i]->toMips()<<"\n";//保证4字节对齐
    }
    return oss.str();
}
