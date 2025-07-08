//
// Created by PengXinyang on 24-11-23.
//

#include "../../../../../../include/value/architecture/user/instruction/IOInstruction/IOPutCh.h"

#include "Register/RegisterController.h"
#include "Register/RegisterTool.h"
#include "structure/Register.h"
#include "structure/text/MipsInstruction/SyscallInstruction.h"
#include "structure/text/PseudoInstruction/LiInstruction.h"
#include "structure/text/PseudoInstruction/MoveInstruction.h"
#include "type/irType/IRVoid.h"

IOPutCh::IOPutCh(Value* op_value)
    : IOInstruction(new IRVoid(),"IOPutCh", "IOInstruction"){
    addOpValue(op_value);
}

string IOPutCh::getIODefine() {
    return "declare void @putch(i32)";
}

string IOPutCh::toLLVM() {
    return "call void @putch(i32 " + opValueChain[0]->value_name + ")";
}

void IOPutCh::generateMIPS() {
    Instruction::generateMIPS();
    //首先将整数加载到寄存器
    Register* target = RegisterController::getRegister(opValueChain[0]);
    target = RegisterTool::loadVarValue(opValueChain[0],target,Register::getRegister(RegisterName::$a0));
    //使用syscall输出整数
    if(target!=Register::getRegister(RegisterName::$a0)) {
        new MoveInstruction(Register::getRegister(RegisterName::$a0),target);
    }
    //11号输出字符
    new LiInstruction(Register::getRegister(RegisterName::$v0),11);
    new SyscallInstruction();
}
