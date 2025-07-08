//
// Created by PengXinyang on 24-12-13.
//

#ifndef DOMINANTTREE_H
#define DOMINANTTREE_H
#include "value/architecture/user/Function.h"

/**
* 求解支配树
* 每个函数都有对应的支配树
* 这里也包括求解支配边
*/
class Function;
class DominantTree {
private:
    Function* function = nullptr;
    //支配集合，求这个基本块支配了哪些基本块。因为基本块在函数中是按照先后顺序存放的，所以这里按照hash存即可
    unordered_map<BasicBlock*,vector<BasicBlock*>> dominateBlocks;
    //支配边
    unordered_map<BasicBlock*,vector<BasicBlock*>> dominateEdge;
    //支配树，本质上支配树是求直接支配：
    //若x严格支配y，并且不存在被x严格支配同时严格支配y的结点
    //父节点
    unordered_map<BasicBlock*, BasicBlock*> parentBlock;
    //支配树子节点
    unordered_map<BasicBlock*, vector<BasicBlock*>> childBlock;
public:
    DominantTree() = default;
    explicit DominantTree(Function* function);
    unordered_map<BasicBlock*,vector<BasicBlock*> >& getDominateBlocks();
    unordered_map<BasicBlock*,vector<BasicBlock*> >& getDominateEdge();
    unordered_map<BasicBlock*, BasicBlock*>& getParentBlock();
    unordered_map<BasicBlock*, vector<BasicBlock*> >& getChildBlock();

    /**
    * 初始化支配集，每个基本块都把自己放进去
    */
    void initDominateBlocks();

    /**
    * 生成该函数的支配集
    * 在for循环中找到所有不被 basicBlock支配的基本块，放入集合中
    * 这样所有不在集合中的基本块，都是被basicBlock支配的基本块
    * 这样可以将这些基本块加入到支配集里
    * 注意，这里生成的是每个节点支配了谁，而不是被支配；
    */
    void generateDominateBlocks();

    /**
     * 利用dfs进行深搜，找到所有不被block支配的基本块，放入集合中
     * 原理：
     * 当我们的入口基本块和所求的目标基本块相等时，函数停止
     * 这意味着集合并不会加入目标基本块block的后继
     * 如果block的某个后继基本块，可以由其它基本块经过，
     * 那么当entry等于这个“其它基本块”时，也会将这个后继加入集合
     * 所以最终没有加入集合的基本块，就是basic支配的基本块，必须通过basic才能到达
     * 与教程给的不同
     * @param entryBlock dfs的入口，调用的时候应当是第一个基本块
     * @param block
     * @param noDominateSet 不被block支配的基本块集合
     */
    void dfsDominateBlocks(BasicBlock* entryBlock, BasicBlock* block, unordered_set<BasicBlock*>& noDominateSet);

    /**
     * 生成支配树，其实就是求直接支配关系
     */
    void generateDominantTree();

    /**
     * 构建支配边界
     * 按照教程给的伪代码遍历即可
     * 首先，遍历函数的所有基本块和后记，记为a和b
     * 令x等于a
     * 然后，当x不严格支配b，将b纳入DF(x)中
     * x更新为x的直接支配者，也就是支配树的父节点
     */
    void generateDominateEdge();

    /**
     * 输出支配树，debug专用
     * @return
     */
    string printDominantTree();
};



#endif //DOMINANTTREE_H
