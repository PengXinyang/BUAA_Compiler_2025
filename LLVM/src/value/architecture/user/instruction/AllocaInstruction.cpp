//
// Created by PengXinyang on 24-11-24.
//

#include "../../../../../include/value/architecture/user/instruction/AllocaInstruction.h"

#include "generate/MipsCell.h"
#include "Register/RegisterController.h"
#include "Register/RegisterTool.h"
#include "structure/text/MipsInstruction/ITypeInstruction.h"
#include "type/irType/IRArray.h"
#include "type/irType/IRPointer.h"

AllocaInstruction::AllocaInstruction(IRType *ir_type, const string& name)
    :Instruction(new IRPointer(ir_type),name,"alloca"){
    this->ir_type = ir_type;
}

string AllocaInstruction::toLLVM() {
    return value_name + " = " + instructionType + " " + ir_type->toLLVM();
}

void AllocaInstruction::generateMIPS() {
    Instruction::generateMIPS();
    // 在栈上分配空间，同时更新栈指针
    int offset = 0;
    if(ir_type->isArray()) {
        if(dynamic_cast<IRArray*>(ir_type)->get_element_type()->isChar()) {
            //offset = -1 * dynamic_cast<IRArray*>(ir_type)->calculate_dims();
            offset = -4 * dynamic_cast<IRArray*>(ir_type)->calculate_dims();
        }else {
            offset = -4 * dynamic_cast<IRArray*>(ir_type)->calculate_dims();
        }
    }else if(ir_type->isChar()) {
        //offset = -1;
        offset = -4;
    }else {
        offset = -4;
    }
    MipsCell::moveNowOffset(offset);
    // 尝试从寄存器中获取当前alloca指令的寄存器
    Register* register_ = RegisterController::getRegister(this);
    if(register_) {
        //如果存在对应的寄存器，那么我们直接将地址赋值给这个寄存器即可
        new ITypeInstruction("addi",register_,Register::getRegister(RegisterName::$sp),MipsCell::getStackOffset());
    }else {
        // 如果不存在对应的寄存器，那么我们需要重新分配一个寄存器
        // 首先使用t8寄存器保存分配空间的首地址（实际上是最低地址）
        new ITypeInstruction("addi",Register::getRegister(RegisterName::$t8),Register::getRegister(RegisterName::$sp),MipsCell::getStackOffset());
        // 然后再从栈上为这个指令开一个空间，保存刚刚新分配空间的首地址
        RegisterTool::allocaRegister(this,Register::getRegister(RegisterName::$t8));
    }
}
