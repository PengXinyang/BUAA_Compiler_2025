//
// Created by PengXinyang on 24-11-24.
//

#include "../../../../../include/value/architecture/user/instruction/StoreInstruction.h"

#include "Register/RegisterController.h"
#include "Register/RegisterTool.h"
#include "structure/Register.h"
#include "structure/text/MipsInstruction/LSTypeInstruction.h"
#include "type/irType/IRVoid.h"

StoreInstruction::StoreInstruction(Value* value, Value* pointer)
    : Instruction(new IRVoid(),"StoreInstruction","store"){
    //使用的操作数有两个
    addOpValue(value);
    addOpValue(pointer);
}

string StoreInstruction::toLLVM() {
    //store i32 %2, i32* %1
    ostringstream os;
    //store
    os<<instructionType<<" ";
    //i32 %2
    os<<opValueChain[0]->value_type->toLLVM()<<" "<<opValueChain[0]->value_name;
    os<<", ";
    //i32* %1
    os<<opValueChain[1]->value_type->toLLVM()<<" "<<opValueChain[1]->value_name;
    return os.str();
}

void StoreInstruction::generateMIPS() {
    Instruction::generateMIPS();
    //sw register_source, RAM_destination
    //该指令第0个操作数是要source，第一个是dst
    Register* srcRegister = RegisterController::getRegister(opValueChain[0]);
    srcRegister = RegisterTool::loadVarValue(opValueChain[0],srcRegister,Register::getRegister(RegisterName::$t8));
    Register* dstRegister = RegisterController::getRegister(opValueChain[1]);
    //目标寄存器一定是指针
    dstRegister = RegisterTool::loadPointerValue(opValueChain[1],dstRegister,Register::getRegister(RegisterName::$t9));
    //调用sw指令
    if(opValueChain[0]->value_type->isChar()) new LSTypeInstruction("sb","",srcRegister,dstRegister,0);
    else new LSTypeInstruction("sw","",srcRegister,dstRegister,0);
}
