//
// Created by PengXinyang on 24-11-23.
//

#include "../../../../../../include/value/architecture/user/instruction/IOInstruction/IOGetInt.h"

#include "generate/MipsCell.h"
#include "Register/RegisterController.h"
#include "Register/RegisterTool.h"
#include "structure/text/MipsInstruction/SyscallInstruction.h"
#include "structure/text/PseudoInstruction/LiInstruction.h"
#include "structure/text/PseudoInstruction/MoveInstruction.h"
#include "type/irType/IRInt.h"
IOGetInt::IOGetInt():IOInstruction(new IRInt(),"IOGetInt", "IOInstruction"){

}
IOGetInt::IOGetInt(const string& name, const string& instruction_type)
    : IOInstruction(new IRInt(),name, instruction_type){

}

string IOGetInt::getIODefine() {
    return "declare i32 @getint()";
}

string IOGetInt::toLLVM() {
    return value_name + " = call i32 @getint()";
}

void IOGetInt::generateMIPS() {
    Instruction::generateMIPS();
    //使用syscall读一个整数
    auto* v0 = Register::getRegister(RegisterName::$v0);
    new LiInstruction(v0,5);
    new SyscallInstruction();
    //将读取的整数装载到寄存器中
    Register *reg = RegisterController::getRegister(this);
    //如果寄存器已经分配，需要移动到v0
    if(reg!=nullptr) {
        new MoveInstruction(reg,v0);
    }else {
        RegisterTool::reAllocaRegister(this,v0);
    }
}
