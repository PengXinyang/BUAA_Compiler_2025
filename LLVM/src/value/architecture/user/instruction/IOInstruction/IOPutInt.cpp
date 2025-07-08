//
// Created by PengXinyang on 24-11-23.
//

#include "../../../../../../include/value/architecture/user/instruction/IOInstruction/IOPutInt.h"

#include "Register/RegisterController.h"
#include "Register/RegisterTool.h"
#include "structure/Register.h"
#include "structure/text/MipsInstruction/SyscallInstruction.h"
#include "structure/text/PseudoInstruction/LiInstruction.h"
#include "structure/text/PseudoInstruction/MoveInstruction.h"
#include "type/irType/IRVoid.h"

IOPutInt::IOPutInt(Value* op_value)
    : IOInstruction(new IRVoid(),"IOPutInt", "IOInstructor"){
    addOpValue(op_value);
}

string IOPutInt::getIODefine() {
    return "declare void @putint(i32)";
}

string IOPutInt::toLLVM() {
    return "call void @putint(i32 " + opValueChain[0]->value_name + ")";
}

void IOPutInt::generateMIPS() {
    Instruction::generateMIPS();
    //首先将整数加载到寄存器
    Register* target = RegisterController::getRegister(opValueChain[0]);
    target = RegisterTool::loadVarValue(opValueChain[0],target,Register::getRegister(RegisterName::$a0));
    //使用syscall输出整数
    if(target!=Register::getRegister(RegisterName::$a0)) {
        new MoveInstruction(Register::getRegister(RegisterName::$a0),target);
    }
    new LiInstruction(Register::getRegister(RegisterName::$v0),1);
    new SyscallInstruction();
}
