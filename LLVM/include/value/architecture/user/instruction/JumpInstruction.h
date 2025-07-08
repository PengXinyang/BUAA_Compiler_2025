//
// Created by PengXinyang on 24-11-28.
//

#ifndef JUMPINSTRUCTION_H
#define JUMPINSTRUCTION_H
#include "value/architecture/user/Instruction.h"


/**
* 用于生成 LLVM IR 中的 br 指令,无条件跳转
* br label <dest>
*/
class JumpInstruction : public Instruction{
private:
    BasicBlock* destination = nullptr;//跳转的<dest>块
    bool isMipsReduce = false;//判断是否需要生成汇编代码,主要用于后端代码优化时去除连接块的不必要跳转语句
public:
    explicit JumpInstruction(BasicBlock* destination);
    [[nodiscard]] BasicBlock* getDestination() const;
    void setDestination(BasicBlock* destination);
    void setIsMipsReduce(bool isMipsReduce);
    string toLLVM() override;
    void generateMIPS() override;
};



#endif //JUMPINSTRUCTION_H
