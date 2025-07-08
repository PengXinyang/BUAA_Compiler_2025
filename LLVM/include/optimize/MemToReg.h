//
// Created by PengXinyang on 24-12-10.
//

#ifndef MEMTOREG_H
#define MEMTOREG_H
#include <stack>

#include "value/architecture/Module.h"

/**
* 用于处理静态单赋值
* 目的：将只使用了load，store的alloca转成寄存器，减少内存开销
*/
class MemToReg {
private:
    //生成LLVM IR的顶级模块
    static Module* module;
    //存储了每个函数的第一个基本块
    static BasicBlock* firstBlock;
    //存储了所有使用该alloca指令的load指令，以及phi指令，维护使用链
    static vector<Instruction*> loadInstructions;
    //存储了所有使用该alloca指令的store指令，以及phi指令，维护定义链
    static vector<Instruction*> storeInstructions;
    //存储所有使用该指令的基本块
    static vector<BasicBlock*> useBlocks;
    //存储所有定义该指令的基本块
    static vector<BasicBlock*> defineBlocks;
    //存储phi指令的值的
    static vector<Value*> phiValueVector;
    //当前的alloca指令
    static Instruction* nowAllocaInstruction;
public:
    static BasicBlock* getFirstBlock();
    static void setFirstBlock(BasicBlock* first_block);

    /**
    * 执行Mem2Reg优化的函数
    * @param module
    */
    static void Mem2Reg(Module* module);

    /**
    * 根据指令，重新配置相关参数
    * @param instruction
    */
    static void resetByInstruction(Instruction* instruction);

    /**
    * 插入phi指令，要求在每个基本块最前面
    * 根据课件给出的算法
    * for v : 原程序中的变量 do
        F ←{}   ▷已插入ϕ函数的基本块集合
        W←{}    ▷v 的定义（包括新插入的ϕ函数）所在的基本块集合
       for d ∈ v 的定义Defs(v) do
            W←W∪{d所在的基本块}
       while W={} do
        从W中选择并删除一个基本块X
        for Y : DF(X) 中的基本块 do
            if Y / ∈ F then
               在基本块Y的入口处插入ϕ函数v←ϕ(...)
                F ←F∪{Y}
                if Y / ∈ Defs(v) then
                    W←W∪{Y}
    */
    static void insertPhiInstruction();

    /**
    * 深搜遍历基本块，对其中的变量重命名
    * 首先，删除该基本块的不必要指令
    * 然后，遍历基本块的后继，
    * 将最新的value(栈顶)填充进每个后继块的第一个phi指令
    * 随后进行深搜
    * 将该次dfs时压入的 stack数据全部弹出
    * 这时候我们需要有支配树
    * 默认从第一个基本块开始进行重命名
    * 注意，不能直接获取支配树的后续，因为存在一种情况：
    * 1->2->3, 1->3, 这种情况1不是直接支配3，但是3确实是1的后继
    * 在基本块3中，很可能有phi指令，所以需要赋值，那么应当获得的是流图的后继
    * @param basic_block
    */
    static void renameVar(BasicBlock* basic_block = firstBlock);

    /**
     * 删除不必要的指令，包括alloca，store，load等
     * 遍历基本块各个指令，修改定义使用链
     * 在支配树上做深度优先遍历，删除alloca 指令，并为其建立堆栈
     * 对于store 指令，将存入的值push到栈顶，并删除
     * 对于load 指令，取出栈顶元素进行替换。
     * 然后查看当前基本块的后继块(CFG)，对所有 phi 指令，从对应的栈顶取出值填入。
     * @param basic_block
     */
    static int removeInstruction(BasicBlock* basic_block);
};



#endif //MEMTOREG_H
