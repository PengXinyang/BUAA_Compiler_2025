//
// Created by PengXinyang on 24-12-13.
//

#ifndef DCE_H
#define DCE_H
#include "value/architecture/BasicBlock.h"

/**
* 删除死代码
* 主要原因是有一些return，跳转指令后的语句，这些没有必要保留
*/
class DCE {
public:
    /**
    * 删除基本块内的分支后代码。
    * 对于一个基本块，跳转/return代码之后的所有代码都不会到达，直接删除即可
    * 而且如果不删除，那么在插入phi函数会出现异常
    * @param basic_block
    */
    static void deleteBranchInstruction(BasicBlock* basic_block);

    /**
    * 删除函数中不可到达的基本块
    * 主要原因是，有些块不可到达，但是后面流图可能会有很多后继，导致构建支配集的时候出现问题
    * 进而影响支配树的生成
    * 采用dfs暴力搜索
    * @param function
    */
    static void deleteBlock(Function* function);
private:
    static void dfsDeleteBlock(BasicBlock* basic_block);
};



#endif //DCE_H
