//
// Created by PengXinyang on 24-12-4.
//

#ifndef JALINSTRUCTION_H
#define JALINSTRUCTION_H
#include "structure/MipsStructure.h"


class JalInstruction :public MipsStructure{
private:
    string block_name;//跳转到哪个块
public:
    JalInstruction() = default;
    explicit JalInstruction(const string &block_name);
    string toMips() override;
};



#endif //JALINSTRUCTION_H
