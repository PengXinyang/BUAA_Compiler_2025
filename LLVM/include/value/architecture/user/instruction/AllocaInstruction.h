//
// Created by PengXinyang on 24-11-24.
//

#ifndef ALLOCAINSTRUCTION_H
#define ALLOCAINSTRUCTION_H
#include "value/architecture/user/Instruction.h"


/*
* 生成LLVM的alloca指令
* 用于申请内存空间
* 示例：
* %3 = alloca i32, align 4
* <result> = alloca <type>
*/
class AllocaInstruction :public Instruction{
private:
    /*
     * 指令申请内存空间的变量类型，
     * 注意，对于Alloca指令本身，它的类型应当是“变量类型的指针”
     */
    IRType* ir_type = nullptr;

public:
    [[nodiscard]] IRType * get_alloca_ir_type() const {
        return ir_type;
    }
    AllocaInstruction() = default;
    AllocaInstruction(IRType* ir_type, const string& name);
    string toLLVM() override;
    void generateMIPS() override;
};



#endif //ALLOCAINSTRUCTION_H
