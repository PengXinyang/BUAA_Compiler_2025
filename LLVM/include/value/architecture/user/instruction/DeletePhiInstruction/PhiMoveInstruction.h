//
// Created by PengXinyang on 24-12-18.
//

#ifndef PhiMoveInstruction_H
#define PhiMoveInstruction_H
#include "value/architecture/user/Instruction.h"

/**
* 将PC指令转换为move指令，便于phi消除后能够转换为mips指令
*/
class PhiMoveInstruction :public Instruction{
private:
    //源寄存器
    Value*source = nullptr;
    //目标寄存器
    Value*destination = nullptr;
public:
    PhiMoveInstruction() = default;
    PhiMoveInstruction(const string& name, Value* source, Value* destination);
    void setSource(Value* source);
    void setDestination(Value* destination);
    [[nodiscard]] Value* getSource() const;
    [[nodiscard]] Value* getDestination() const;
    string toLLVM() override;
    void generateMIPS() override;
};



#endif //PhiMoveInstruction_H
