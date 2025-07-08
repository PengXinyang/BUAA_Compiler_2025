//
// Created by PengXinyang on 24-11-28.
//

#ifndef BRINSTRUCTION_H
#define BRINSTRUCTION_H
#include "value/architecture/BasicBlock.h"
#include "value/architecture/user/Instruction.h"

/*
* 用于生成 LLVM IR 中的 br 指令,
* 主要用于生成条件跳转指令
* br label <dest>
* br i1 <cond>, label <iftrue>, label <iffalse>
* 示例：
* br label %1（这种属于无条件跳转，放到jump指令中）
* br i1 %3, label %4, label %7
*/
class BrInstruction :public Instruction{
private:
    Value* condition = nullptr; //条件
    BasicBlock*ifTrueBlock = nullptr;//条件为真的基本块
    BasicBlock* ifFalseBlock = nullptr;//条件为假的基本块
public:
    [[nodiscard]] Value * getCondition() const {
        return condition;
    }

    void setCondition(Value *condition) {
        this->condition = condition;
        opValueChain[0] = condition;
    }

    [[nodiscard]] BasicBlock * getIfTrueBlock() const {
        return ifTrueBlock;
    }

    void setIfTrueBlock(BasicBlock *if_true_block) {
        ifTrueBlock = if_true_block;
        opValueChain[1] = if_true_block;
    }

    [[nodiscard]] BasicBlock * getIfFalseBlock() const {
        return ifFalseBlock;
    }

    void setifFalseBlock(BasicBlock *else_block) {
        ifFalseBlock = else_block;
        opValueChain[2] = else_block;
    }
    BrInstruction(Value* condition, BasicBlock* ifTrueBlock, BasicBlock* ifFalseBlock);
    string toLLVM() override;
    void generateMIPS() override;
};



#endif //BRINSTRUCTION_H
