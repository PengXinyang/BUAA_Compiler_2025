//
// Created by PengXinyang on 24-12-5.
//

#ifndef MTYPEINSTRUCTION_H
#define MTYPEINSTRUCTION_H
#include <vector>

#include "structure/MipsStructure.h"
#include "structure/Register.h"

/**
* MTypeAsm 是.text段中的mult,div,mflo,mfhi,mthi,mtlo汇编
* 用于处理乘除语句
*/
class MTypeInstruction :public MipsStructure{
private:
    string operation_name;//操作名称
    Register* rs = nullptr;//rs寄存器
    Register* rd = nullptr;//rd寄存器
    Register* rt = nullptr;//rt寄存器
public:
    MTypeInstruction() = default;
    MTypeInstruction(const string &operation_name, const vector<Register*>& registers);
    string toMips() override;
};



#endif //MTYPEINSTRUCTION_H
