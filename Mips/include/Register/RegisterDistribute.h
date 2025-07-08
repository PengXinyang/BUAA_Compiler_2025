//
// Created by PengXinyang on 24-12-18.
//

#ifndef REGISTERDISTRIBUTE_H
#define REGISTERDISTRIBUTE_H
#include <vector>

#include "structure/Register.h"
#include "value/Value.h"
#include "value/architecture/BasicBlock.h"
#include "value/architecture/user/Instruction.h"

/**
* 处理寄存器分配的问题
*/
class RegisterDistribute {
private:
    //当前分配器可以分配的寄存器，这里使用t0-s7寄存器
    static vector<Register*> registers;
    //保存value和寄存器的映射
    static unordered_map<Value*, Register*> ValueToRegisterMap;
    //保存寄存器和value的映射
    static unordered_map<Register*, Value*> RegisterToValueMap;
public:
    /**
    * 初始化可以使用的寄存器： t0-s7
    * t8和t9用于没有寄存器可分配的时候，进行分配
    */
    static void init();
    static void setValueToRegisterMap(const unordered_map<Value*, Register*>& valueToRegisterMap);
    static void setRegisterToValueMap(const unordered_map<Register*, Value*>& registerToValueMap);
    static unordered_map<Value*, Register*>& getValueToRegisterMap();

    /**
     * 在基本块内进行寄存器的分配
     * 首先，遍历所有的指令，记录每个变量在该块中最后一次被使用的位置
     * 然后，第二次遍历，释放寄存器
     * 遍历所有直接支配的节点，调用buildRegisterMap方法
     * 然后，将该基本块定义的变量对应的寄存器释放
     * 最后，将“后继不再使用但是是从indBasicBlock传过来”的变量对应的寄存器映射恢复回来
     * 即：在used中，但是不是在当前基本块定义的变量
     * @param block 
     */
    static void distributeInBasicBlock(BasicBlock* block);
private:
    /**
    * 遍历寄存器列表，找到一个尚未分配的寄存器
    * @return
    */
    static Register* distributeRegister();

    /**
    * 释放寄存器
    * 如果该指令的某个op是该基本块内最后一次引用，并且out不包含这个value
    * 说明作用域已经结束，可以暂时释放，不改变ValueToReg
    * 如果是定义语句，那么需要分配一个寄存器
    */
    static void releaseRegister(BasicBlock* block, Instruction* instruction,
        unordered_map<Value*, Value*>& lastUse,
        unordered_set<Value*>& defined,
        unordered_set<Value*>& used);

    /**
     * 如果当前个寄存器所对应的变量在其child中不会被使用到(即in中不包含该变量)
     * 可以记录这个映射关系到RVMap中，并释放该寄存器，
     * 当为child（包括child的child）分配完寄存器后，
     * 再将RVMap中的映射关系恢复，以免影响其兄弟节点的寄存器分配
     * 之后为其child调用该函数实现递归调用
     * 最后在回溯过程中将RVMap中存储的被删除的映射关系恢复
     * @param block 
     */
    static void buildRegisterMap(BasicBlock* block);
};



#endif //REGISTERDISTRIBUTE_H
