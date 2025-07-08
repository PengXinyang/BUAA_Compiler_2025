//
// Created by PengXinyang on 24-12-6.
//

#ifndef MIPSCELL_H
#define MIPSCELL_H
#include <unordered_map>

#include "structure/Register.h"
#include "value/Value.h"
#include "value/architecture/user/Function.h"


class MipsCell {
private:
    //当前栈指针偏移量
    static int stackOffset;
    //记录Value和栈指针偏移量的映射
    static std::unordered_map<Value*, int> stackOffsetMap;
public:
    //移动当前栈指针偏移量
    static void moveNowOffset(int offset);
    //每次进入一个函数，都需要重置相关属性
    static void resetFunction(Function* function);
    static int getStackOffset();
    static void addValueOffset(Value* value, int offset);
    static int getValueOffset(Value* value);
    //获取已分配的寄存器
    static vector<Register*> getRegisterDistribute();
};



#endif //MIPSCELL_H
