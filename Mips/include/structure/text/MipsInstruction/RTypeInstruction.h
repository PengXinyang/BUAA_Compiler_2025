//
// Created by PengXinyang on 24-12-5.
//

#ifndef RTYPEINSTRUCTION_H
#define RTYPEINSTRUCTION_H
#include <vector>

#include "structure/MipsStructure.h"
#include "structure/Register.h"

/**
* R型汇编代码
* 包括 add,sub,addu,subu,and,or,xor,nor,slt,sltu,
* sllv,srlv,srav
* slt,sle,sgt,sge,seq,sne
* jr
*/
class RTypeInstruction :public MipsStructure{
private:
    string operation_name;
    Register* rs = nullptr;
    Register* rt = nullptr;
    Register* rd = nullptr;
public:
    RTypeInstruction() = default;
    RTypeInstruction(const string &operation_name, const vector<Register*> &registers);
    string toMips() override;
};



#endif //RTYPEINSTRUCTION_H
