//
// Created by PengXinyang on 24-11-24.
//

#ifndef TRUNKINSTRUCTION_H
#define TRUNKINSTRUCTION_H
#include "value/architecture/user/Instruction.h"


/*
* 截断指令?当 int 类型值赋给 char 类型变量时,可以直接使用 trunc 指令将 i32 转换为 i8
* 仿照zext写
* <result> = trunc <ty> <value> to <ty2>
* 示例：
* %10 = trunc i32 %9 to i8
*/
class TruncInstruction : public Instruction{
private:
    IRType* final_type = nullptr;
public:
    TruncInstruction()= default;
    TruncInstruction(const string& name, Value* value, IRType* final_type);
    string toLLVM() override;
    void generateMIPS() override;
};



#endif //TRUNKINSTRUCTION_H
