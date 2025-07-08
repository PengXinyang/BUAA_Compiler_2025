//
// Created by PengXinyang on 24-11-28.
//

#include "../../../../../include/value/architecture/user/instruction/IcmpInstruction.h"

#include "Register/RegisterController.h"
#include "Register/RegisterTool.h"
#include "structure/Register.h"
#include "structure/text/MipsInstruction/RTypeInstruction.h"
#include "type/irType/IRBool.h"

IcmpInstruction::IcmpInstruction(const string& name, const string& instruction_type, Value *op1, Value *op2)
    : Instruction(new IRBool(),name, instruction_type){
    addOpValue(op1);
    addOpValue(op2);
}

string IcmpInstruction::toLLVM() {
    /*
     * %4 = icmp ne i8 %3, 0
     * %5 = icmp ne i32 %4, %6
     */
    return value_name + " = icmp " + instructionType + " " + opValueChain[0]->value_type->toLLVM()
    + " "+opValueChain[0]->value_name + ", " + opValueChain[1]->value_name;
}

void IcmpInstruction::generateMIPS() {
    Instruction::generateMIPS();
    //仿照加减运算实现即可，这里涉及的都是R型指令
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
    string op_name = instructionType;
    if(op_name[0]!='s') {
        op_name = "s" + op_name;
    }
    new RTypeInstruction(op_name,registers);
    //如果没有分配寄存器，现在重新分配寄存器
    RegisterTool::reAllocaRegister(this,rd);
}
