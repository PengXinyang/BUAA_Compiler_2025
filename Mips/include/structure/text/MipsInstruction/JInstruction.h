//
// Created by PengXinyang on 24-12-4.
//

#ifndef JINSTRUCTION_H
#define JINSTRUCTION_H
#include "structure/MipsStructure.h"

/**
* 直接跳转，j指令
*/
class JInstruction :public MipsStructure{
private:
    string block_name;//跳转到哪个块
public:
    JInstruction() = default;
    explicit JInstruction(const string &block_name);
    string toMips() override;
};



#endif //JINSTRUCTION_H
