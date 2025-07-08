//
// Created by PengXinyang on 24-12-6.
//

#include "../../include/Register/RegisterTool.h"

#include "generate/MipsCell.h"
#include "Register/RegisterController.h"
#include "structure/text/MipsInstruction/ITypeInstruction.h"
#include "structure/text/MipsInstruction/LSTypeInstruction.h"
#include "structure/text/MipsInstruction/RTypeInstruction.h"
#include "structure/text/PseudoInstruction/LaInstruction.h"
#include "structure/text/PseudoInstruction/LiInstruction.h"
#include "value/architecture/ConstValue.h"
#include "value/architecture/user/GlobalVariable.h"
#include <algorithm>
#include <climits>
#include "structure/text/PseudoInstruction/MoveInstruction.h"
#include "type/irType/IRArray.h"
#include "type/irType/IRChar.h"
#include "type/irType/IRPointer.h"
IRType* getRealType(Value* value) {
    if (value == nullptr) { return nullptr;}
    IRType* ir_type = value->value_type;
    //去除指针的影响
    /*while(instanceof<IRPointer>(ir_type)) {
        ir_type = dynamic_cast<IRPointer*>(ir_type)->ir_type();
    }*/
    if(ir_type->isArray()) {
        //如果是数组，考察内部元素的变量
        ir_type=dynamic_cast<IRArray*>(ir_type)->get_element_type();
    }
    return ir_type;
}
int RegisterTool::moveValue(Value *value) {
    int all_offset = 0;
    if (value == nullptr) {return all_offset;}
    IRType* realType = getRealType(value);
    //字符类型，移动一位
    /*if(instanceof<IRChar>(realType))MipsCell::moveNowOffset(-1);
    else*/ MipsCell::moveNowOffset(-4);
    all_offset = MipsCell::getStackOffset();
    MipsCell::addValueOffset(value, all_offset);
    return all_offset;
}

void RegisterTool::allocaRegister(Value *value, Register *reg) {
    moveValue(value);
    IRType* realType = getRealType(value);
    if(instanceof<IRChar>(realType)) {
        //说明要按照byte存取
        new LSTypeInstruction("sb","",reg,Register::getRegister(RegisterName::$sp),MipsCell::getStackOffset());
    }
    else new LSTypeInstruction("sw","",reg,Register::getRegister(RegisterName::$sp),MipsCell::getStackOffset());
}

void RegisterTool::reAllocaRegister(Value *value, Register *reg) {
    if(RegisterController::getRegister(value) == nullptr) {
        allocaRegister(value, reg);
    }
}

void RegisterTool::memoryAllocaRegister(Value *value, Register *reg) {
    if(RegisterController::getRegister(value) == nullptr) {
        int offset = MipsCell::getValueOffset(value);
        if(offset==INT_MAX) {
            //说明其实没找到value
            allocaRegister(value, reg);
        }else {
            // 由于调用这个函数对应的是move指令，
            // 所以即使存在对应的offset我们也需要更新该处offset对应的value
            if(IRType* realType = getRealType(value); instanceof<IRChar>(realType)) {
                //说明要按照byte存取
                new LSTypeInstruction("sb","",reg,Register::getRegister(RegisterName::$sp),offset);
            }
            new LSTypeInstruction("sw","",reg,Register::getRegister(RegisterName::$sp),offset);
        }
    }
}

Register *RegisterTool::loadRegister(Value *op, Register *reg, Register *reg_instead) {
    Register* register_ = reg;
    if(register_ == nullptr) {
        //如果没有对应的寄存器，则使用默认寄存器
        register_ = reg_instead;
        // 尝试在栈中查找操作数
        int offset = MipsCell::getValueOffset(op);
        // 如果没有找到，则移动栈指针并添加对应的偏移量和value的映射
        if(offset==INT_MAX) {
            offset = moveValue(op);
        }
        if(IRType* realType = getRealType(op); instanceof<IRChar>(realType)) {
            //说明要按照byte存取
            new LSTypeInstruction("lb","",register_,Register::getRegister(RegisterName::$sp),offset);
        }
        else new LSTypeInstruction("lw","",register_,Register::getRegister(RegisterName::$sp),offset);
    }
    return register_;
}

Register *RegisterTool::loadVarValue(Value *op, Register *reg, Register *reg_instead) {
    Register* register_ = reg;
    if(instanceof<ConstValue>(op)) {
        //说明是立即数，需要取
        new LiInstruction(reg_instead, dynamic_cast<ConstValue*>(op)->getNum());
        return reg_instead;
    }
    //否则加载到寄存器中
    register_ = loadRegister(op, register_, reg_instead);
    return register_;
}

Register *RegisterTool::loadPointerValue(Value *op, Register *reg, Register *reg_instead) {
    Register* register_ = reg;
    if(instanceof<GlobalVariable>(op)) {
        //全局变量，直接使用la
        new LaInstruction(reg_instead,op->value_name.substr(1));
        return reg_instead;
    }
    //否则加载到寄存器中
    register_ = loadRegister(op, register_, reg_instead);
    return register_;
}

Register *RegisterTool::loadMemoryOffset(Value *op, Register *reg, Register *reg_instead, Register *pointerReg, Register *offsetReg,IRType* irType) {
    Register* register_ = offsetReg;
    if(instanceof<IRPointer>(irType)) irType=dynamic_cast<IRPointer*>(irType)->ir_type();
    if(instanceof<ConstValue>(op)) {
        //从数组中取值，需要考虑常量是什么类型的
        int offset = dynamic_cast<ConstValue*>(op)->getNum();
        if(irType->isChar()) offset *= 1;
        else offset*=4;
        new ITypeInstruction("addi",reg,pointerReg,offset);
    }else {
        register_ = loadRegister(op, register_, reg_instead);
        //如果是int，左移两位表示*4；如果是char则不用移位
        if(irType->isChar()) {reg_instead = register_;}
        else new ITypeInstruction("sll",reg_instead,register_,2);
        vector<Register*> registers;
        registers.push_back(reg);
        registers.push_back(reg_instead);
        registers.push_back(pointerReg);
        new RTypeInstruction("addu",registers);
    }
    return register_;
}

Register *RegisterTool::allocParamReg(Value *param, Register *paramReg, int currentOffset, vector<Register *> allocatedRegs) {
    IRType* realType = getRealType(param);
    if(instanceof<ConstValue>(param)) {
        //用立即数取
        new LiInstruction(paramReg, dynamic_cast<ConstValue*>(param)->getNum());
    }
    else if(RegisterController::getRegister(param) == nullptr) {
        if(instanceof<IRChar>(realType)) {
            new LSTypeInstruction("lb","",paramReg,Register::getRegister(RegisterName::$sp),MipsCell::getValueOffset(param));
        }
        else new LSTypeInstruction("lw","",paramReg,Register::getRegister(RegisterName::$sp),MipsCell::getValueOffset(param));
    }else {
        const Register* register_ = RegisterController::getRegister(param);
        if(instanceof<Param>(param)) {
            //说明是参数类型
            if (const auto it = find(allocatedRegs.begin(), allocatedRegs.end(), register_); it != allocatedRegs.end()) {
                const int index = static_cast<int>(std::distance(allocatedRegs.begin(), it)); // 获取索引
                if(instanceof<IRChar>(realType)) {
                    //不用乘4，因为只有1字节
                    new LSTypeInstruction("lb", "", paramReg, Register::getRegister(RegisterName::$sp), currentOffset - (index + 1)*4);
                }
                else new LSTypeInstruction("lw", "", paramReg, Register::getRegister(RegisterName::$sp), currentOffset - (index+1)*4);
            }
        }else {
            new MoveInstruction(paramReg, RegisterController::getRegister(param));
        }
    }
    return paramReg;
}

Register *RegisterTool::allocParamMem(Value *param, Register *paramReg, int currentOffset, vector<Register *> allocatedRegs, int param_num) {
    Register* register_ = paramReg;
    IRType* realType = getRealType(param);
    if(instanceof<ConstValue>(param)) {
        new LiInstruction(paramReg, dynamic_cast<ConstValue*>(param)->getNum());
    }
    else if(RegisterController::getRegister(param) != nullptr) {
        Register* register_2 = RegisterController::getRegister(param);
        if(instanceof<Param>(param)) {
            if (const auto it = find(allocatedRegs.begin(), allocatedRegs.end(), register_2); it != allocatedRegs.end()) {
                const int index = static_cast<int>(std::distance(allocatedRegs.begin(), it)); // 获取索引
                if(instanceof<IRChar>(realType)) {
                    new LSTypeInstruction("lb", "", paramReg, Register::getRegister(RegisterName::$sp), currentOffset - (index + 1)*4);
                }
                else new LSTypeInstruction("lw", "", paramReg, Register::getRegister(RegisterName::$sp), currentOffset - (index+1)*4);
            }
        }else {
            register_ = register_2;
        }
    }else {
        if(instanceof<IRChar>(realType)) {
            new LSTypeInstruction("lb", "", paramReg, Register::getRegister(RegisterName::$sp), MipsCell::getValueOffset(param));
        }
        else new LSTypeInstruction("lw","",register_,Register::getRegister(RegisterName::$sp),MipsCell::getValueOffset(param));
    }
    if(instanceof<IRChar>(realType)) {
        new LSTypeInstruction("sb","",register_,Register::getRegister(RegisterName::$sp),currentOffset - (static_cast<int>(allocatedRegs.size()) + 2 + param_num)*4);
    }
    else new LSTypeInstruction("sw","",register_,Register::getRegister(RegisterName::$sp),currentOffset - (static_cast<int>(allocatedRegs.size()) + 2 + param_num)*4);
    return register_;
}
