//
// Created by PengXinyang on 24-12-18.
//

#include "../../../../../../include/value/architecture/user/instruction/DeletePhiInstruction/PhiMoveInstruction.h"

#include "Register/RegisterController.h"
#include "Register/RegisterTool.h"
#include "structure/Register.h"
#include "structure/text/PseudoInstruction/MoveInstruction.h"
#include "type/irType/IRVoid.h"

PhiMoveInstruction::PhiMoveInstruction(const string& name, Value *source, Value *destination)
    :Instruction(new IRVoid(),name,"move"){
    this->source = source;
    this->destination = destination;
    addOpValue(destination);
    addOpValue(source);
}

Value *PhiMoveInstruction::getDestination() const {
    return destination;
}
Value *PhiMoveInstruction::getSource() const {
    return source;
}
void PhiMoveInstruction::setDestination(Value *destination) {
    this->destination = destination;
}
void PhiMoveInstruction::setSource(Value *source) {
    this->source = source;
}
string PhiMoveInstruction::toLLVM() {
    return "move "+ destination->value_type->toLLVM() + " " + destination->value_name + ", " + source->value_name;
}

void PhiMoveInstruction::generateMIPS() {
    Instruction::generateMIPS();
    //获取两者对应分配的mips寄存器
    Register* source_register = RegisterController::getRegister(source);
    Register* destination_register = RegisterController::getRegister(destination);
    //如果两个寄存器都存在且相同，则可以无需生成move
    if(source_register && destination_register && source_register==destination_register) {
        return;
    }
    if(!destination_register) destination_register = Register::getRegister(RegisterName::$t8);
    source_register = RegisterTool::loadVarValue(source,source_register,destination_register);
    if(source_register!=destination_register) {
        //两个寄存器确实不相等，这时生成move指令
        new MoveInstruction(destination_register,source_register);
    }
    //如果使用了默认寄存器，那么我们需要重新申请空间
    RegisterTool::memoryAllocaRegister(destination,destination_register);
}
