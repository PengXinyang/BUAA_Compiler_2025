//
// Created by PengXinyang on 24-11-28.
//

#ifndef ICMPINSTRUCTION_H
#define ICMPINSTRUCTION_H
#include "value/architecture/user/Instruction.h"


/**
* 用于生成 LLVM IR 中的 icmp 指令,
* 继承于 Instruction,主要用于生成比较指令
* <result> = icmp <cond> <ty> <op1>, <op2>
* 示例：
* %3 = icmp ne i32 %2, 0
* 即判断%2是否不等于0
* 比较op1和op2是否满足条件，返回i1
*/
class IcmpInstruction :public Instruction{
public:
    /**
    * 构造函数
    * 示例：
    * %3 = icmp ne i32 %2, 0
    * @param name 寄存器的名称，如示例的%3
    * @param instruction_type 指令的类型，比如eq, ne, sgt, sge, slt, sle
    * @param op1 第一个比较的数
    * @param op2 第二个比较的数
    */
    IcmpInstruction(const string& name, const string& instruction_type, Value* op1, Value* op2);
    string toLLVM() override;
    void generateMIPS() override;
};



#endif //ICMPINSTRUCTION_H
