//
// Created by PengXinyang on 24-12-5.
//

#ifndef SYSCALLINSTRUCTION_H
#define SYSCALLINSTRUCTION_H
#include "structure/MipsStructure.h"


class SyscallInstruction :public MipsStructure{
public:
    SyscallInstruction();
    string toMips() override;
};



#endif //SYSCALLINSTRUCTION_H
