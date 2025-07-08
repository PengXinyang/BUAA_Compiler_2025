//
// Created by PengXinyang on 24-12-6.
//

#ifndef REGISTERTOOL_H
#define REGISTERTOOL_H
#include "structure/Register.h"
#include "value/Value.h"

/**
* 工具类，用于处理寄存器的分配
*/
class RegisterTool {
public:
    /**
    * 将一个Value移动-4的偏移量，返回移动后的总偏移量
    * 并且将value和偏移添加到map中
    * 在本阶段，采用栈式存储，所有变量都存在栈里，根据value和offset的映射map来取值
    */
    static int moveValue(Value* value);

    /**
    * 申请分配的寄存器
    * 1. 首先将栈指针向下移动4个字节，用来分配寄存器
    * 2. 然后将value的偏移量和寄存器的映射添加到寄存器控制器中
    * @param value
    * @param reg
    */
    static void allocaRegister(Value* value, Register* reg);

    /**
    * 申请重新分配寄存器
    * @param value
    * @param reg
    */
    static void reAllocaRegister(Value* value, Register* reg);

    /**
    * 在内存中申请分配寄存器
    * @param value
    * @param reg
    */
    static void memoryAllocaRegister(Value* value, Register* reg);

    /**
    * 加载寄存器的值
    * @param op
    * @param reg value对应的寄存器
    * @param reg_instead 没有reg，要使用替代的寄存器
    * @return
    */
    static Register* loadRegister(Value* op, Register* reg, Register* reg_instead);

    /**
    * 加载变量的值
    * @param op
    * @param reg
    * @param reg_instead
    * @return
    */
    static Register* loadVarValue(Value* op,Register* reg, Register* reg_instead);

    /**
    * 加载指针的值
    * @param op
    * @param reg
    * @param reg_instead
    * @return
    */
    static Register* loadPointerValue(Value* op,Register* reg, Register* reg_instead);

    /**
    * 加载内存偏移量，其中irType表示取的哪种类型，因为char数组是byte
    * @param op
    * @param reg
    * @param reg_instead
    * @param pointerReg
    * @param offsetReg
    * @return
    */
    static Register* loadMemoryOffset(Value* op,Register* reg, Register* reg_instead,Register* pointerReg, Register* offsetReg, IRType* irType);

    /**
    * 在函数参数中分配寄存器
    * @param param
    * @param paramReg
    * @param currentOffset
    * @param allocatedRegs
    * @return
    */
    static Register* allocParamReg(Value* param, Register* paramReg,int currentOffset, vector<Register*> allocatedRegs);

    /**
    * 在函数参数中分配内存
    * @param param
    * @param paramReg
    * @param currentOffset
    * @param allocatedRegs
    * @param param_num
    * @return
    */
    static Register* allocParamMem(Value* param, Register* paramReg,int currentOffset, vector<Register*> allocatedRegs, int param_num);
};



#endif //REGISTERTOOL_H
