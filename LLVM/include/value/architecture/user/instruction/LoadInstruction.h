//
// Created by PengXinyang on 24-11-24.
//

#ifndef LOADINSTRUCTION_H
#define LOADINSTRUCTION_H
#include "type/irType/IRPointer.h"
#include "value/architecture/user/Instruction.h"

/*
* LLVM的load指令
* 示例：
* %8 = load i32, i32* %3, align 4
* 相当于在%3存了一个数，然后取出来
* 存的类型和取得类型只有一个指针的差距
* <result> = load <ty>, ptr <pointer>
* <result> 是目标变量，用于存储从 <pointer> 指向的内存地址中加载的值。
* <ty> 是加载的值的类型。
* <pointer> 是一个指针，指向存储 ty 类型值的内存地址。
* 先不考虑类型转换
*/
class LoadInstruction :public Instruction{
public:
    LoadInstruction() = default;
    LoadInstruction(const string& name,Value* ptr_value);
    string toLLVM() override;
    //lw register_destination, RAM_source
    void generateMIPS() override;
};



#endif //LOADINSTRUCTION_H
