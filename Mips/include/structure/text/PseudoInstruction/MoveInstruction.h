//
// Created by PengXinyang on 24-12-5.
//

#ifndef MOVEINSTUCTION_H
#define MOVEINSTUCTION_H
#include "structure/MipsStructure.h"
#include "structure/Register.h"

/**
* 汇编中对应move，
*/
class MoveInstruction :public MipsStructure{
private:
    Register*dst = nullptr;//目标寄存器
    Register*src = nullptr;//源寄存器
public:
    MoveInstruction() = default;
    MoveInstruction(Register* dst, Register* src);
    string toMips() override;
};



#endif //MOVEINSTUCTION_H
