//
// Created by PengXinyang on 24-12-12.
//

#ifndef CFG_H
#define CFG_H
#include <unordered_map>
#include "value/architecture/user/Function.h"


/**
* 控制流图，为了方便处理，以函数为基准
*/
class BasicBlock;
class Function;
class CfgGraph {
private:
    //流入的基本块，这里面存的就是每个基本块的前序基本块
    unordered_map<BasicBlock*,vector<BasicBlock*>> inBlocks;
    //流出的基本块，这里面存的就是每个基本块的后序基本块
    unordered_map<BasicBlock*,vector<BasicBlock*>> outBlocks;
    Function* function = nullptr;
public:
    CfgGraph() = default;
    explicit CfgGraph(Function* function);
    unordered_map<BasicBlock*,vector<BasicBlock*>>& getInBlocks();
    unordered_map<BasicBlock*,vector<BasicBlock*>>& getOutBlocks();
    /**
    * 构造这个函数的控制流图
    */
    void buildCFG();
    /**
    * 在两个基本块之间添加边，分别加入前驱map和后驱map
    * @param from_block 从哪个block来
    * @param to_block 到哪个block去
    */
    void addEdge(BasicBlock* from_block, BasicBlock* to_block);

    /**
     * 向两个基本块中间插入一个基本块
     * 用于消除phi指令
     * @param from_block 前序基本块
     * @param to_block 后序基本块
     * @param mid_block 中间基本块
     */
    void insertBlock(BasicBlock* from_block, BasicBlock* to_block, BasicBlock* mid_block);

    /**
     * 将两个基本块进行合并，修改对应的in和out
     * @param from_block
     * @param to_block
     */
    static void mergeBlock(BasicBlock* from_block, BasicBlock* to_block);

    /**
    * 输出控制流图，debug用
    */
    string printCFG();
private:
    /**
    * 向流图添加前序基本块
    * @param from_block 从哪个block来
    * @param to_block 到哪个block去
    * @param index 将基本块插入到哪个位置？一般用于在两个基本块之间插
    */
    void addInBlock(BasicBlock* to_block,BasicBlock* from_block, int index = -1);
    /**
    * 向流图添加后序基本块
    * @param from_block 从哪个block来
    * @param to_block 到哪个block去
    * @param index 将基本块插入到哪个位置？一般用于在两个基本块之间插
    */
    void addOutBlock(BasicBlock* from_block,BasicBlock* to_block, int index = -1);


};



#endif //CFG_H
