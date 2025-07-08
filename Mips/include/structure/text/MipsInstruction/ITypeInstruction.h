//
// Created by PengXinyang on 24-12-5.
//

#ifndef ITYPEINSTRUCTION_H
#define ITYPEINSTRUCTION_H
#include "structure/MipsStructure.h"
#include "structure/Register.h"

/**
* i型指令，包括addi,addiu,andi,ori,xori,slti,sltiu,lui,sll,srl,sra
* rs 是源寄存器, rd 是目标寄存器, number 是立即数
*/
class ITypeInstruction :public MipsStructure{
private:
    string operation_name;//操作名称
    Register* rs = nullptr;//rs寄存器
    Register* rd = nullptr;//rd寄存器
    int number = -1;//立即数
public:
    ITypeInstruction() = default;
    ITypeInstruction(const string &operation_name, Register* rd, Register* rs,  int number);
    string toMips() override;
};



#endif //ITYPEINSTRUCTION_H
