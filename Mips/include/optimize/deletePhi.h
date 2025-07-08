//
// Created by PengXinyang on 24-12-18.
//

#ifndef DELETEPHI_H
#define DELETEPHI_H
#include "value/architecture/Module.h"
#include "value/architecture/user/Function.h"
#include "value/architecture/user/instruction/DeletePhiInstruction/PhiMoveInstruction.h"

/**
* 主要用于phi消除，消除后才能转换为mips指令
* 主要是封装了pc转move的情况
*/
class deletePhi {
public:
    /**
    * 消除phi的启动类
    */
    static void DeletePhi(Module* module);
    /**
    * 生成临时变量的temp move
    * 由于可能存在循环和冲突，所以需要引入临时寄存器，保证串行执行后和并行一致
    * @param function
    * @param phi_move_instructions
    */
    static vector<PhiMoveInstruction*> generateMoveTemp(Function* function, vector<PhiMoveInstruction*>& phi_move_instructions);
private:
    static Module* module;
    /**
    * 生成循环的move指令集合
    * 比如： move a, b;  move c, a;，这种串行化后会改变PC的语义，所以需要引入临时寄存器
    * 首先，检查每个move指令之后的指令，如果这个指令的dest是某个move的src，那么存在循环赋值的问题
    * 需要增加中间变量，即将所有source使用dest的move指令全部替换为临时寄存器，
    * 最后在moveList开头添加临时寄存器的指令
    * @param function
    * @param phi_move_instructions
    * @return
    */
    static vector<PhiMoveInstruction*> generateMoveCycle(Function* function, vector<PhiMoveInstruction*>& phi_move_instructions);

    /**
     * 用于生成共享寄存器的Move指令集合
     * 1.检查该指令之前的所有指令，
     * 如果source对应的reg同时是某一个move的dst的reg，那么存在寄存器冲突的问题
     * 2. 如果出现了寄存器冲突的情况，我们需要增加中间变量，
     * 将所有使用作为dest的move指令，改为临时今存其，并在开头添加寄存器
     * @param function
     * @param phi_move_instructions 
     * @return 
     */
    static vector<PhiMoveInstruction*> generateMoveSameRegister(Function* function, vector<PhiMoveInstruction*>& phi_move_instructions);
};



#endif //DELETEPHI_H
