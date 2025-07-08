//
// Created by PengXinyang on 24-12-19.
//

#ifndef ACTIVEVARANALYSIS_H
#define ACTIVEVARANALYSIS_H
#include "value/architecture/user/Function.h"

/**
* 活跃变量分析，按照课本的方法
* 活跃变量关注的是基本块，和流图类似，所以还是以函数为单位
* 为冲突图做准备
*/
class BasicBlock;
class Function;
class ActiveVarAnalysis {
private:
    Function*function = nullptr;
    unordered_map<BasicBlock*, unordered_set<Value*>> use;
    unordered_map<BasicBlock*, unordered_set<Value*>> def;
    unordered_map<BasicBlock*, unordered_set<Value*>> in;
    unordered_map<BasicBlock*, unordered_set<Value*>> out;
public:
    ActiveVarAnalysis() = default;
    explicit ActiveVarAnalysis(Function* function);
    unordered_set<Value*>& getUseVar(BasicBlock* block);
    unordered_set<Value*>& getDefVar(BasicBlock* block);
    unordered_set<Value*>& getInVar(BasicBlock* block);
    unordered_set<Value*>& getOutVar(BasicBlock* block);

    /**
    * 遍历所有的基本块，生成use和def集合
    */
    void generateUseDef();

    /**
    * 活跃变量分析
    * 首先，最初的out均为空
    * in = use + (out-def)，集合运算
    * out是后继的in的并集
    * 当in不再发生变化时，退出循环
    */
    void generateInOut();

    /**
    * debug使用
    * @return
    */
    string printActiveAnalysis() const;
};



#endif //ACTIVEVARANALYSIS_H
