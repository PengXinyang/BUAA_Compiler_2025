//
// Created by PengXinyang on 24-11-24.
//

#include "../../../../../include/value/architecture/user/instruction/ReturnInstruction.h"

#include "Register/RegisterController.h"
#include "Register/RegisterTool.h"
#include "structure/Register.h"
#include "structure/text/MipsInstruction/RTypeInstruction.h"
#include "structure/text/PseudoInstruction/MoveInstruction.h"
#include "type/irType/IRVoid.h"

ReturnInstruction::ReturnInstruction(Value* return_value)
    :Instruction(new IRVoid(),"ReturnInstruction","ret"){
    if(return_value != nullptr) {
        addOpValue(return_value);
    }
}

Value *ReturnInstruction::getReturnValue() const {
    if(opValueChain.empty()) {
        return nullptr;
    }
    return opValueChain.front();
}

string ReturnInstruction::toLLVM() {
    const Value *return_value = getReturnValue();
    if(return_value == nullptr) {
        return instructionType+" void";
    }
    return instructionType+" "+return_value->value_type->toLLVM() + " "+return_value->value_name;
}

void ReturnInstruction::generateMIPS() {
    Instruction::generateMIPS();
    //如果存在返回值，需要存在v0中
    Value* return_value = getReturnValue();
    if(return_value != nullptr) {
        //有返回值，存在v0
        Register* return_register = RegisterController::getRegister(return_value);
        return_register = RegisterTool::loadVarValue(return_value,return_register,Register::getRegister(RegisterName::$v0));
        if(return_register != Register::getRegister(RegisterName::$v0)) {
            //将值转移到v0中
            new MoveInstruction(Register::getRegister(RegisterName::$v0),return_register);
        }
    }
    //跳转出函数,jr隶属于R指令
    vector<Register*> registers;
    registers.push_back(Register::getRegister(RegisterName::$ra));
    new RTypeInstruction("jr",registers);
}
