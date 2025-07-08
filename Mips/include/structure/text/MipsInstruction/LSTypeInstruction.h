//
// Created by PengXinyang on 24-12-5.
//

#ifndef LSTYPEINSTRUCTURE_H
#define LSTYPEINSTRUCTURE_H
#include "structure/MipsStructure.h"
#include "structure/Register.h"

/**
* 存储类汇编代码,包括lb,lbu,lh,lhu,lw,sb,sh,sw
*/
class LSTypeInstruction :public MipsStructure{
private:
    string operation_name;//操作名
    string label;//标签名
    Register* rd = nullptr;//目标寄存器
    Register* base = nullptr;//基址寄存器
    int offset = 0;//地址偏移量
public:
    LSTypeInstruction() = default;
    LSTypeInstruction(const string &operation_name, const string &label, Register* rd, Register* base, int offset);
    string toMips() override;
};



#endif //LSTYPEINSTRUCTURE_H
