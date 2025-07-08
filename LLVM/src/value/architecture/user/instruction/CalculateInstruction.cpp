//
// Created by PengXinyang on 24-11-24.
//

#include "../../../../../include/value/architecture/user/instruction/CalculateInstruction.h"

#include "Register/RegisterController.h"
#include "Register/RegisterTool.h"
#include "structure/Register.h"
#include "structure/text/MipsInstruction/MTypeInstruction.h"
#include "structure/text/MipsInstruction/RTypeInstruction.h"
#include "value/architecture/ConstValue.h"

CalculateInstruction::CalculateInstruction(IRType* ir_type, const string& name, const string& instruction_type, Value *v1, Value *v2)
    : Instruction(ir_type,name, instruction_type){
    addOpValue(v1);
    addOpValue(v2);
}

string CalculateInstruction::toLLVM() {
    ostringstream oss;
    //%9 = add nsw i32 %5, %8
    oss<<value_name+" = ";//%9
    oss<<instructionType<<" ";//"add nsw"，nsw表示假定不会溢出；如果溢出就是未定义行为
    oss<<value_type->toLLVM()<<" ";//i32
    oss<<opValueChain[0]->value_name<<", "<<opValueChain[1]->value_name;//%5,%8
    return oss.str();
}

int CalculateInstruction::getConstNum() const {
    int is_const0 = 0,is_const1 = 0;
    if(instanceof<ConstValue>(opValueChain[0])) is_const0 = 1;
    if(instanceof<ConstValue>(opValueChain[1])) is_const1 = 1;
    return is_const0 + is_const1;
}

void CalculateInstruction::generateMIPS() {
    Instruction::generateMIPS();
    // 首先在寄存器控制器中查找当前指令对应的寄存器，如果为null则使用t8寄存器
    Register* rd = RegisterController::getRegister(this);
    if(!rd) rd = Register::getRegister(RegisterName::$t8);
    // 同理，我们也需要查找当前指令的操作数对应的寄存器
    Register* rs = RegisterController::getRegister(opValueChain[0]);
    Register* rt = RegisterController::getRegister(opValueChain[1]);
    rs = RegisterTool::loadVarValue(opValueChain[0],rs,Register::getRegister(RegisterName::$t8));
    rt = RegisterTool::loadVarValue(opValueChain[1],rt,Register::getRegister(RegisterName::$t9));
    vector<Register*> registers;
    registers.push_back(rd);
    registers.push_back(rs);
    registers.push_back(rt);
    vector<Register*> MRegisters;
    MRegisters.push_back(rs);
    MRegisters.push_back(rt);
    vector<Register*> MfRegisters;
    MfRegisters.push_back(rd);
    if(instructionType=="add") {
        new RTypeInstruction("addu",registers);
    }else if(instructionType=="sub") {
        new RTypeInstruction("subu",registers);
    }else if(instructionType=="mul") {
        new MTypeInstruction("mult",MRegisters);
        new MTypeInstruction("mflo",MfRegisters);
    }else if(instructionType=="sdiv") {
        new MTypeInstruction("div",MRegisters);
        new MTypeInstruction("mflo",MfRegisters);
    }else if(instructionType=="srem") {
        new MTypeInstruction("div",MRegisters);
        new MTypeInstruction("mfhi",MfRegisters);
    }else if(instructionType=="and") {
        new RTypeInstruction("and",registers);
    }else if(instructionType=="or") {
        new RTypeInstruction("or",registers);
    }else {
        printf("Unknown instruction type in CalculateInstruction\n");
    }
    // 如果之前没有为当前指令分配寄存器，那么我们需要重新分配一个寄存器
    RegisterTool::reAllocaRegister(this,rd);
}
