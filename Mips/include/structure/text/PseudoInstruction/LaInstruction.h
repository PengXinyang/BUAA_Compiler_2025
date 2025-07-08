//
// Created by PengXinyang on 24-12-5.
//

#ifndef LAINSTRUCTION_H
#define LAINSTRUCTION_H
#include "structure/MipsStructure.h"
#include "structure/Register.h"

/**
* la指令
*/
class LaInstruction :public MipsStructure{
private:
    Register* rd = nullptr;//目标寄存器
    string label;//标签名
public:
    LaInstruction() = default;
    LaInstruction(Register* rd, const string &label);
    string toMips() override;
};



#endif //LAINSTRUCTION_H
