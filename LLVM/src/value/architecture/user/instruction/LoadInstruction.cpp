//
// Created by PengXinyang on 24-11-24.
//

#include "../../../../../include/value/architecture/user/instruction/LoadInstruction.h"

#include <sstream>

#include "Register/RegisterController.h"
#include "Register/RegisterTool.h"
#include "structure/Register.h"
#include "structure/text/MipsInstruction/LSTypeInstruction.h"
#include "type/irType/IRChar.h"

LoadInstruction::LoadInstruction(const string& name,Value* ptr_value)
    :Instruction(ptr_value->value_type,
        name,"load"){
    //只是用了一个操作数
    if(dynamic_cast<IRPointer*>(ptr_value->value_type)) {
        //说明是指针类型，将该指令改为指针类型
        value_type = dynamic_cast<IRPointer*>(ptr_value->value_type)->ir_type();
    }
    addOpValue(ptr_value);
}

string LoadInstruction::toLLVM() {
    //<result> = load <ty>, ptr <pointer>
    //%8 = load i32, i32* %3
    ostringstream os;
    os<<value_name<<" = ";//result，%8
    os<<instructionType<<" ";//load
    os<<value_type->toLLVM()<<", ";//type，i32
    //使用的操作数只有一个
    if(instanceof<IRPointer>(opValueChain[0]->value_type)) {
        os<<opValueChain[0]->value_type->toLLVM()<<" ";//ptr，i32*
    }
    else  os<<opValueChain[0]->value_type->toLLVM()<<"* ";//ptr，i32*
    os<<opValueChain[0]->value_name;//pointer，%3
    return os.str();
}

void LoadInstruction::generateMIPS() {
    Instruction::generateMIPS();
    //首先 load 指令的第0个操作数一定是指针，需要先读取指针的值
    //将作为基址寄存器
    Register* pointer_register = RegisterController::getRegister(opValueChain[0]);
    //加载指针，如果寄存器为空就用t8替代
    pointer_register = RegisterTool::loadPointerValue(opValueChain[0],pointer_register,Register::getRegister(RegisterName::$t8));
    //查找当前指令有没有绑定的寄存器，如果没有就使用t8
    Register* rd = RegisterController::getRegister(this);
    if(!rd) rd = Register::getRegister(RegisterName::$t8);
    if(instanceof<IRChar>(value_type)) {
        //使用lb读
        new LSTypeInstruction("lb","",rd,pointer_register,0);
    }
    else new LSTypeInstruction("lw","",rd,pointer_register,0);
    RegisterTool::reAllocaRegister(this,rd);
}
