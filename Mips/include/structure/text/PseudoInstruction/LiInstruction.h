//
// Created by PengXinyang on 24-12-5.
//

#ifndef LIINSTRUCTION_H
#define LIINSTRUCTION_H
#include "structure/MipsStructure.h"
#include "structure/Register.h"

/**
* li指令，取立即数
*/
class LiInstruction :public MipsStructure{
private:
    Register* rd = nullptr;//目标寄存器
    int number = 0;//取哪个立即数
public:
    LiInstruction() = default;
    LiInstruction(Register* rd, int number);
    string toMips() override;
};



#endif //LIINSTRUCTION_H
