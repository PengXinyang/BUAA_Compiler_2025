//
// Created by PengXinyang on 24-12-4.
//

#ifndef MIPSBLOCK_H
#define MIPSBLOCK_H
#include "structure/MipsStructure.h"


class MipsBlock :public MipsStructure{
private:
    string block_name;//跳转到哪个块
public:
    MipsBlock() = default;
    explicit MipsBlock(const string &block_name);
    string toMips() override;
};



#endif //MIPSBLOCK_H
