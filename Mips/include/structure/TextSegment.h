//
// Created by PengXinyang on 24-12-7.
//

#ifndef TEXTSEGMENT_H
#define TEXTSEGMENT_H
#include "Segment.h"


class TextSegment : public Segment{
public:
    string toMips() override;
};



#endif //TEXTSEGMENT_H
