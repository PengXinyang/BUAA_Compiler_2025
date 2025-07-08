//
// Created by PengXinyang on 24-11-23.
//

#include "../../../../../../include/value/architecture/user/instruction/IOInstruction/IOPutStr.h"

#include "structure/text/MipsInstruction/SyscallInstruction.h"
#include "structure/text/PseudoInstruction/LaInstruction.h"
#include "structure/text/PseudoInstruction/LiInstruction.h"
#include "type/irType/IRVoid.h"

IOPutStr::IOPutStr(ConstString* constString)
    : IOInstruction(new IRVoid,"IOPutSt","IOInstruction"){
    this->constString = constString;
}

string IOPutStr::getIODefine() {
    return "declare void @putstr(i8*)";
}

string IOPutStr::toLLVM() {
    /*
     * 示例：
     * call void @putstr(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.1, i64 0, i64 0))
     */
    string const_sting_name = constString->value_name;
    IRPointer* const_string_pointer = constString->getIRPointer();
    return "call void @putstr(i8* getelementptr inbounds ("+
            const_string_pointer->ir_type()->toLLVM() + ", "+
            const_string_pointer->toLLVM() + " " +
            const_sting_name + ", i64 0, i64 0))";
}

void IOPutStr::generateMIPS() {
    Instruction::generateMIPS();
    // 首先我们需要将字符串的地址加载到寄存器中
    // 然后我们使用 syscall 指令输出字符串
    new LaInstruction(Register::getRegister(RegisterName::$a0),constString->value_name.substr(1));
    new LiInstruction(Register::getRegister(RegisterName::$v0),4);
    new SyscallInstruction();
}

