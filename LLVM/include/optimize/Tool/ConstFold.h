//
// Created by PengXinyang on 24-12-12.
//

#ifndef CONSTFOLD_H
#define CONSTFOLD_H
#include "value/architecture/BasicBlock.h"
#include "value/architecture/user/instruction/CalculateInstruction.h"

/**
* 用于处理常量折叠，对于一个基本块内进行处理
* 常量折叠隶属于GVN和LVN优化
*/
class ConstFold {
public:
    /**
    * 计算指令无外乎有0个立即数，1个立即数，2个立即数，所以分三种情况处理即可
    */
    static void handleCalculateInstruction(BasicBlock* basic_block);
private:
    /**
    * 0个立即数, 如果为null, 则表示没有折叠
    * 简单处理，如果两个操作数的指针都相同，那么优化减法和除法
    */
    static Value* handle0(CalculateInstruction* calculate_instruction);
    /**
    * 1个立即数, 如果为null, 则表示没有折叠
    * 主要是用于优化a+0，m*0或1，0/c，0%d，c/1，d%1
    */
    static Value* handle1(CalculateInstruction* calculate_instruction);
    /**
    * 2个立即数, 如果为null, 则表示没有折叠
    * 这种直接计算，然后移出指令，换成常数
    */
    static Value* handle2(CalculateInstruction* calculate_instruction);
};



#endif //CONSTFOLD_H
