//
// Created by PengXinyang on 24-11-24.
//

#include "../../../../../include/value/architecture/user/instruction/CallInstruction.h"

#include "generate/MipsCell.h"
#include "Register/RegisterController.h"
#include "Register/RegisterTool.h"
#include "structure/text/MipsInstruction/ITypeInstruction.h"
#include "structure/text/MipsInstruction/JalInstruction.h"
#include "structure/text/MipsInstruction/LSTypeInstruction.h"
#include "structure/text/PseudoInstruction/MoveInstruction.h"

CallInstruction::CallInstruction(const string& name, Function *call_function, vector<Value *> &args)
    : Instruction(call_function->getReturnType(),name,"call"){
    addOpValue(call_function);
    this->call_function = call_function;
    for(const auto arg : args) {
        addOpValue(arg);
    }
}

Function *CallInstruction::getCallFunction() const {
    return call_function;
}

string CallInstruction::toLLVM() {
    //%17 = call i32 @f(i32 %16)
    //call void @g(i32 %19)
    ostringstream os;
    if(value_type->isVoid()) {
        //如果返回的类型是void，那么采用第二行字符串的方式输出
        os<<"call void ";
    }else {
        //%17 = call i32
        os<<value_name<<" = call "<<value_type->toLLVM()<<" ";
    }
    //第一个使用的操作数一定是函数本身
    os<<opValueChain[0]->value_name<<"(";
    //接下来，将实参数填入字符串
    int param_size = static_cast<int>(opValueChain.size());
    for(int i=1;i<param_size;i++) {
        //i32 %16
        os<<opValueChain[i]->value_type->toLLVM()<<" "<<opValueChain[i]->value_name;
        //如果后面还有参数，就需要加一个逗号
        if(i+1<param_size) {
            os<<", ";
        }
    }
    os<<")";
    return os.str();
}

void CallInstruction::generateMIPS() {
    Instruction::generateMIPS();
    vector<Register *> registers = MipsCell::getRegisterDistribute();
    //将已经分配了寄存器的部分进行压栈
    int offset = 0;
    int stackOffset = MipsCell::getStackOffset();
    for(int i=0;i<registers.size();i++) {
        offset += 4;
        if(i+1<opValueChain.size() && opValueChain[i+1]->value_type->isChar()) {
            //第0个操作数是函数名，所以理论上应当是第一个操作数对应第0个寄存器
            //char用sb存
            new LSTypeInstruction("sb","",registers[i],Register::getRegister(RegisterName::$sp),stackOffset - offset);
        }
        else new LSTypeInstruction("sw","",registers[i],Register::getRegister(RegisterName::$sp),stackOffset - offset);
    }
    //最后将SP指针和RA指针也进行压栈
    new LSTypeInstruction("sw","",Register::getRegister(RegisterName::$sp),Register::getRegister(RegisterName::$sp),stackOffset - offset - 4);
    new LSTypeInstruction("sw","",Register::getRegister(RegisterName::$ra),Register::getRegister(RegisterName::$sp),stackOffset - offset - 8);
    //将实参的值压入被调用函数的堆栈或者寄存器中
    for(int paramNum = 1;paramNum < opValueChain.size();paramNum++) {
        Value* param = opValueChain[paramNum];
        // 如果参数在前3个中，我们直接将他们放入a1-a3寄存器中
        if(paramNum<=3) {
            RegisterTool::allocParamReg(
                param,
                Register::getRegister(Register::regTransform(static_cast<int>(RegisterName::$a0)+paramNum)),
                stackOffset, registers
            );
        }else {
            // 如果参数不在前3个中或寄存器控制器中没有对应寄存器，需要将其存入堆栈中
            RegisterTool::allocParamMem(
                param,
                Register::getRegister(RegisterName::$t8),
                stackOffset, registers, paramNum);
        }
    }
    //将sp设置为被调用函数的栈底地址
    new ITypeInstruction("addi",Register::getRegister(RegisterName::$sp),
        Register::getRegister(RegisterName::$sp),stackOffset - offset - 8);
    //调用函数
    new JalInstruction(opValueChain[0]->value_name.substr(1));
    //恢复sp和ra
    new LSTypeInstruction("lw","",Register::getRegister(RegisterName::$ra),Register::getRegister(RegisterName::$sp),0);
    new LSTypeInstruction("lw","",Register::getRegister(RegisterName::$sp),Register::getRegister(RegisterName::$sp),4);
    //恢复其余寄存器
    for(int i = 0;i<registers.size();i++) {
        if(i+1<opValueChain.size() && opValueChain[i+1]->value_type->isChar()) {
            new LSTypeInstruction("lb","",registers[i],Register::getRegister(RegisterName::$sp),stackOffset - (i + 1) * 4);
        }
        else new LSTypeInstruction("lw","",registers[i],Register::getRegister(RegisterName::$sp),stackOffset - (i + 1) * 4);
    }
    //如果当前函数有返回值，则从v0中获取返回值
    if(!RegisterController::getRegister(this)) {
        //找不到当前函数寄存器，分配v0
        RegisterTool::allocaRegister(this,Register::getRegister(RegisterName::$v0));
    }else {
        new MoveInstruction(RegisterController::getRegister(this),Register::getRegister(RegisterName::$v0));
    }
}
