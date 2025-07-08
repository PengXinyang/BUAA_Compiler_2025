//
// Created by PengXinyang on 24-12-5.
//

#ifndef BTYPEINSTRUCTION_H
#define BTYPEINSTRUCTION_H
#include "structure/MipsStructure.h"
#include "structure/Register.h"

/**
* 比较类型指令，包括 beq,bne,bgtz,blez,bgez,bltz
* rs 是源寄存器rs
* rt 是源寄存器rt
* label 是标签
*/
class BTypeInstruction :public MipsStructure{
private:
    string operation_name;//操作名称, beq等
    Register* rs = nullptr;//rs寄存器
    Register* rt = nullptr;//rt寄存器
    string label;//标签名
public:
    BTypeInstruction() = default;
    BTypeInstruction(const string &operation_name, Register* rs, Register* rt, const string &label);
    string toMips() override;
};



#endif //BTYPEINSTRUCTION_H
