//
// Created by PengXinyang on 24-12-7.
//

#ifndef DATASEGMENT_H
#define DATASEGMENT_H
#include "Segment.h"


class DataSegment : public Segment{
public:
    string toMips() override;
};



#endif //DATASEGMENT_H
