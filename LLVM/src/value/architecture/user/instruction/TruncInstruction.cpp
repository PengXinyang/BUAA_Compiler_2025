//
// Created by PengXinyang on 24-11-24.
//

#include "../../../../../include/value/architecture/user/instruction/TruncInstruction.h"

#include "generate/MipsCell.h"
#include "Register/RegisterController.h"
#include "Register/RegisterTool.h"
#include "structure/text/MipsInstruction/ITypeInstruction.h"
#include "structure/text/MipsInstruction/LSTypeInstruction.h"
#include "structure/text/PseudoInstruction/LiInstruction.h"
#include "value/architecture/ConstValue.h"
#include <climits>

TruncInstruction::TruncInstruction(const string &name, Value *value, IRType *final_type)
    : Instruction(final_type, name, "trunc"){
    addOpValue(value);
    this->final_type = final_type;
}

string TruncInstruction::toLLVM() {
    //%10 = trunc i32 %9 to i8
    return value_name+" = "+instructionType+" "
    + opValueChain[0]->value_type->toLLVM() + " " + opValueChain[0]->value_name
    + " to " + final_type->toLLVM();
}

//TODO: 不确定这么做是否能真的达到强制类型转换的效果
void TruncInstruction::generateMIPS() {
    Instruction::generateMIPS();
    if(final_type->isChar()) {
        Register* register_ = RegisterController::getRegister(opValueChain[0]);
        // 如果有对应的寄存器，我们只需要在栈指针上记录对应的偏移量即可，
        // 否则我们需要重新申请空间
        if(register_==nullptr) {
            register_ = Register::getRegister(RegisterName::$t8);
        }
        if(opValueChain[0]->value_type->isInt()) {
            //说明应该将32位转成8位
            //如果操作数是常数，不从栈帧取，而是直接加载到一个寄存器
            if(instanceof<ConstValue>(opValueChain[0])) {
                new LiInstruction(register_,dynamic_cast<ConstValue*>(opValueChain[0])->getNum());
            }
            else {
                //首先，要将这个数从栈帧取出来
                int offset = MipsCell::getValueOffset(opValueChain[0]);
                if(offset==INT_MAX ||
                    register_ == Register::getRegister(RegisterName::$a1) ||
                    register_ == Register::getRegister(RegisterName::$a2) ||
                    register_ == Register::getRegister(RegisterName::$a3)) {
                    //说明这个值没有存在栈中，或者是a1-a3这种不需要从栈中取
                    }
                else new LSTypeInstruction("lw","",register_,Register::getRegister(RegisterName::$sp),offset);
            }
            new ITypeInstruction("andi",register_,register_,0XFF);
            //存到栈中
            RegisterTool::allocaRegister(this, register_);
            MipsCell::addValueOffset(this,MipsCell::getValueOffset(opValueChain[0]));
        }
    }
}
