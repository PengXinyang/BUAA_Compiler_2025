//
// Created by PengXinyang on 24-11-24.
//

#include "../../../../../include/value/architecture/user/instruction/ZextInstruction.h"

#include "generate/MipsCell.h"
#include "Register/RegisterController.h"
#include "Register/RegisterTool.h"
#include "structure/Register.h"
#include "structure/text/MipsInstruction/ITypeInstruction.h"
#include "structure/text/MipsInstruction/LSTypeInstruction.h"
#include "structure/text/PseudoInstruction/LiInstruction.h"
#include "value/architecture/ConstValue.h"
#include <climits>

ZextInstruction::ZextInstruction(const string& name, Value *value, IRType *final_type)
    : Instruction(final_type, name, "zext"){
    this->final_type = final_type;
    addOpValue(value);
}

string ZextInstruction::toLLVM() {
    //%12 = zext i8 %11 to i32
    return value_name+" = "+instructionType+" "
    + opValueChain[0]->value_type->toLLVM() + " " + opValueChain[0]->value_name
    + " to " + final_type->toLLVM();
}

void ZextInstruction::generateMIPS() {
    Instruction::generateMIPS();
    if(final_type->isInt()) {
        Register* register_ = RegisterController::getRegister(opValueChain[0]);
        // 如果有对应的寄存器，我们需要在栈指针上记录对应的偏移量，
        // 否则我们需要重新申请空间
        if(register_==nullptr) {
            register_ = Register::getRegister(RegisterName::$t8);
        }
        if(opValueChain[0]->value_type->isChar()) {
            //说明应该将低8位转成32位
            //如果操作数是常数，不从栈帧取，而是直接加载到一个寄存器
            if(instanceof<ConstValue>(opValueChain[0])) {
                new LiInstruction(register_,dynamic_cast<ConstValue*>(opValueChain[0])->getNum());
            }
            else {
                int offset = MipsCell::getValueOffset(opValueChain[0]);
                //new LSTypeInstruction("lbu","",register_,Register::getRegister(RegisterName::$sp),offset);
                if(offset==INT_MAX ||
                    register_ == Register::getRegister(RegisterName::$a1) ||
                    register_ == Register::getRegister(RegisterName::$a2) ||
                    register_ == Register::getRegister(RegisterName::$a3)) {
                    //说明这个值没有存在栈中，或者是a1-a3这种不需要从栈中取
                }
                //取的时候，是取一个字符，用lb
                else new LSTypeInstruction("lb","",register_,Register::getRegister(RegisterName::$sp),offset);
            }
            new ITypeInstruction("andi",register_,register_,0XFF);
            RegisterTool::allocaRegister(this, register_);
            MipsCell::addValueOffset(this,MipsCell::getValueOffset(opValueChain[0]));
        }
        else {
            //应当是bool转int，bool我使用sw和lw存取，所以其实就是重新存一下
            int offset = MipsCell::getValueOffset(opValueChain[0]);
            if(offset==INT_MAX ||
                    register_ == Register::getRegister(RegisterName::$a1) ||
                    register_ == Register::getRegister(RegisterName::$a2) ||
                    register_ == Register::getRegister(RegisterName::$a3)) {
                //说明这个值没有存在栈中，或者是a1-a3这种不需要从栈中取
                    }
            else new LSTypeInstruction("lw","",register_,Register::getRegister(RegisterName::$sp),offset);
            RegisterTool::allocaRegister(this, register_);
            MipsCell::addValueOffset(this,MipsCell::getValueOffset(opValueChain[0]));
        }
    }
}
