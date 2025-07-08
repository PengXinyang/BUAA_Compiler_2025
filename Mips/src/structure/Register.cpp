//
// Created by PengXinyang on 24-12-4.
//

#include "../../include/structure/Register.h"

static const string regNames[] = {
    "$zero",
    "$at",
    "$v0", "$v1",
    "$a0", "$a1", "$a2", "$a3",
    "$t0", "$t1", "$t2", "$t3", "$t4", "$t5", "$t6", "$t7",
    "$s0", "$s1", "$s2", "$s3", "$s4", "$s5", "$s6", "$s7",
    "$t8", "$t9",
    "$k0", "$k1",
    "$gp",
    "$sp",
    "$fp",
    "$ra"
};
unordered_map<RegisterName,Register*> Register::registers;
Register::Register(const RegisterName reg) {
    this->reg = reg;
    registers[reg] = this;
}

string Register::toMips()  {
    return regNames[static_cast<int>(reg)];
}

RegisterName Register::getRegisterName() const {
    return reg;
}
Register* Register::getRegister(const RegisterName reg) {
    if(registers.find(reg) == registers.end()) {
        //说明没有找到
        registers[reg] = new Register(reg);
    }
    return registers[reg];
}