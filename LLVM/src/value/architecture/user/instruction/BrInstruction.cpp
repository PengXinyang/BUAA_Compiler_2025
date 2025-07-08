//
// Created by PengXinyang on 24-11-28.
//

#include "../../../../../include/value/architecture/user/instruction/BrInstruction.h"

#include "Register/RegisterController.h"
#include "Register/RegisterTool.h"
#include "structure/Register.h"
#include "structure/text/MipsInstruction/BTypeInstruction.h"
#include "structure/text/MipsInstruction/JInstruction.h"
#include "type/irType/IRVoid.h"

BrInstruction::BrInstruction(Value *condition, BasicBlock *ifTrueBlock, BasicBlock *ifFalseBlock)
    :Instruction(new IRVoid(),"BrInstruction","br"){
    this->condition = condition;
    this->ifTrueBlock = ifTrueBlock;
    this->ifFalseBlock = ifFalseBlock;
    addOpValue(condition);
    addOpValue(ifTrueBlock);
    addOpValue(ifFalseBlock);
}

string BrInstruction::toLLVM() {
    return instructionType + " i1 " + condition->value_name + ", label %" +
                ifTrueBlock->value_name + ", label %" + ifFalseBlock->value_name;
}

void BrInstruction::generateMIPS() {
    Instruction::generateMIPS();
    // 首先在寄存器控制器中查找当前指令对应的寄存器，如果为null则使用t8寄存器
    Register* register_ = RegisterController::getRegister(opValueChain[0]);
    register_ = RegisterTool::loadRegister(opValueChain[0],register_,Register::getRegister(RegisterName::$t8));
    // 判断条件是否不等于0，如果不等于0则跳转到thenBlock，否则跳转到elseBlock
    new BTypeInstruction("bne",register_,Register::getRegister(RegisterName::$zero),opValueChain[1]->value_name);
    new JInstruction(opValueChain[2]->value_name);
}
