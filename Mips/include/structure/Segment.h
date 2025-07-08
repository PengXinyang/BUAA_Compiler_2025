//
// Created by PengXinyang on 24-12-4.
//

#ifndef SEGMENT_H
#define SEGMENT_H
#include <vector>

#include "MipsStructure.h"

/**
* 数据段和代码段统一使用
*/
class Segment {
public:
    vector<MipsStructure*> MipsSegments;
    virtual ~Segment() = default;

    Segment() = default;
    void addMipsSegment(MipsStructure* newMipsSegment);
    virtual string toMips();
};



#endif //SEGMENT_H
