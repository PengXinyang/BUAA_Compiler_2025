//
// Created by PengXinyang on 24-11-28.
//

#include "../../../../../include/value/architecture/user/instruction/JumpInstruction.h"

#include "structure/text/MipsInstruction/JInstruction.h"
#include "type/irType/IRVoid.h"
#include "value/architecture/BasicBlock.h"

JumpInstruction::JumpInstruction(BasicBlock *destination)
    : Instruction(new IRVoid(),"JumpInstruction","jump"){
    this->destination = destination;
    this->isMipsReduce = false;
    addOpValue(destination);
}

BasicBlock *JumpInstruction::getDestination() const {
    return destination;
}

void JumpInstruction::setDestination(BasicBlock *destination) {
    this->destination = destination;
    opValueChain[0] = destination;
}

string JumpInstruction::toLLVM() {
    return "br label %" + opValueChain[0]->value_name;
}

void JumpInstruction::setIsMipsReduce(const bool isMipsReduce) {
    this->isMipsReduce = isMipsReduce;
}

void JumpInstruction::generateMIPS() {
    Instruction::generateMIPS();
    //跳转到目标基本块
    new JInstruction(opValueChain[0]->value_name);
}
