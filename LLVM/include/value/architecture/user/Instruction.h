//
// Created by PengXinyang on 24-11-23.
//

#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include "value/user/User.h"

class BasicBlock;
class Instruction :public User{
private:
    BasicBlock* blockParent = nullptr;//标记这条指令所在的基本块
protected:
    string instructionType;//指令的名字，比如是add指令还是sub指令,trunk等
public:
    string& getInstructionType(){
        return instructionType;
    }
    void setBlockParent(BasicBlock* currentBlock) {
        blockParent = currentBlock;
    }
    [[nodiscard]] BasicBlock* getBlockParent() const {
        return blockParent;
    }
    Instruction() = default;
    Instruction(IRType* ir_type,const string& name, const string &instructionType);
    void generateMIPS() override;

    //-----------------------以下是代码优化部分---------------------//
    void insertPhiInstruction() override;

    /**
    * 对于该指令，生成其use集合和def集合，用于活跃变量
    * 首先，看该指令使用了哪些操作数
    * 如果定义链不包括这些变量，并且它们属于指令，全局变量或参数，那么它们应当纳入使用链
    * 随后看使用链是否包括自己，如果不包括，应当纳入定义链中
    */
    void generateUseDef();

    /**
    * 判断这个指令是否有用
    * 必须能够被“使用”，才能加入use-def集
    * @return
    */
    bool isUseful();
};



#endif //INSTRUCTION_H
