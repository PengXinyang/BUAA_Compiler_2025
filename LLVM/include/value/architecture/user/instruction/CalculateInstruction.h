//
// Created by PengXinyang on 24-11-24.
//

#ifndef CALCULATEINSTRUCTION_H
#define CALCULATEINSTRUCTION_H
#include "value/architecture/user/Instruction.h"

/*
* 计算指令，包括：
* <result> = add <ty> <op1>, <op2> 加
* <result> = sub <ty> <op1>, <op2> 减
* <result> = mul <ty> <op1>, <op2> 乘
* <result> = sdiv <ty> <op1>, <op2> 有符号除
* <result> = srem <type> <op1>, <op2> 有符号模
* <result> = and <ty> <op1>, <op2> 与
* <result> = or <ty> <op1>, <op2> 或
*/
class CalculateInstruction :public Instruction{
public:
    CalculateInstruction() = default;
    //由于计算部分涉及到int和char的加减乘除，所以类型只能之后确定再赋
    CalculateInstruction(IRType* ir_type, const string& name, const string& instruction_type, Value* v1, Value* v2);
    string toLLVM() override;
    //指令里操作数有几个是常数
    [[nodiscard]] int getConstNum() const;
    void generateMIPS() override;
};



#endif //CALCULATEINSTRUCTION_H
