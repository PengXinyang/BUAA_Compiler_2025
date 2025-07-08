//
// Created by PengXinyang on 24-11-24.
//

#ifndef STOREINSTRUCTION_H
#define STOREINSTRUCTION_H
#include "value/architecture/user/Instruction.h"


/*
* LLVM的store指令
* 示例：
* store i32 %2, i32* %1
* 含义：将%2对应的数存入到%1中
* store <ty> <value>, ptr <pointer>
* 注意：这个指令本身属于IRVoid
*/
class StoreInstruction : public Instruction{
public:
    StoreInstruction() = default;
    //store <ty> <value>, ptr <pointer>,将value存到pointer中
    StoreInstruction(Value* value,Value* pointer);
    string toLLVM() override;
    //存指令sw
    void generateMIPS() override;
};



#endif //STOREINSTRUCTION_H
