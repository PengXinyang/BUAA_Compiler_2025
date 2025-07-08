//
// Created by PengXinyang on 24-11-24.
//

#ifndef RETURNINSTRUCTION_H
#define RETURNINSTRUCTION_H
#include "value/architecture/user/Instruction.h"


/*
* Return指令
* ret <type> <value> 或 ret void
* 实际上可以统一存一个return_value，根据这个类型看返回什么
* 如果是void类型，就不输出value
* 示例：ret i32 %9
* 注意：本身这个指令属于IRVoid
*/
class ReturnInstruction : public Instruction{
public:
    ReturnInstruction() = default;
    explicit ReturnInstruction(Value* return_value);
    [[nodiscard]] Value* getReturnValue() const;
    string toLLVM() override;
    void generateMIPS() override;
};



#endif //RETURNINSTRUCTION_H
