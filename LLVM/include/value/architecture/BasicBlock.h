//
// Created by PengXinyang on 24-11-23.
//

#ifndef BASICBLOCK_H
#define BASICBLOCK_H
#include <unordered_map>
#include <unordered_set>

#include "value/Value.h"

class Function;
class Instruction;
class Loop;
//基本块类，一个函数有多个基本块，至少有一个
class BasicBlock :public Value{
    //一个基本块内有多个指令,且不能包括跳转和返回指令。
    //一个基本块内的指令，要么都执行，要么都不执行
    //先不考虑代码优化
private:
    vector<Instruction*> instructions;//多条指令按照先后顺序存入数组
    Function* functionParent = nullptr;//这个基本块属于哪个函数
    bool is_exist = true;//基本块是否还存在
    Loop* loopParent = nullptr;//这个基本块是不是某个循环下的
public:
    BasicBlock() = default;
    explicit BasicBlock(const string& name);
    //添加一条指令，如果index在当前vector范围内则插入到这个地方；否则添加在最后
    void addInstruction(Instruction* instruction,int index = -1);
    //判断指令集是否为空
    [[nodiscard]] bool isEmpty() const {
        return instructions.empty();
    }
    //获取最后一条指令
    [[nodiscard]] Instruction* getLastInstruction() const {
        return instructions.back();
    }
    //获取倒数第二条指令
    [[nodiscard]] Instruction* getPenuInstruction() const {
        if(instructions.size()<=1) return nullptr;
        return instructions[instructions.size()-2];
    }
    //设置指令集
    void setInstructions(const vector<Instruction*>& instructions);
    //获取指令集
    vector<Instruction*>& getInstructions();
    //设置exist状态
    void setExist(bool exist);
    //获取exist状态
    [[nodiscard]] bool isExist() const;
    //设置属于哪个循环块
    void setLoopParent(Loop* loop);
    //获取当前属于哪个循环块
    [[nodiscard]] Loop* getLoopParent() const;
    //当前在循环第几层深度？
    [[nodiscard]] int getLoopDepth() const;
    //设置当前属于哪个函数
    void setFunctionParent(Function* function);
    //获取当前块属于哪个函数
    [[nodiscard]] Function* getFunctionParent() const;
    //覆写，基本块应当如何输出
    string toLLVM() override;
    //生成汇编，之后再补
    void generateMIPS() override;

    //------------------以下用于代码优化-----------------//
    //删除无用代码
    void DCEBlockInstruction();
    //获取该基本块的前序基本块
    vector<BasicBlock*>& getInBlocks();
    //获取该基本块的后序基本块
    vector<BasicBlock*>& getOutBlocks();
    //获取支配集合
    vector<BasicBlock*>& getDominateBlocks();
    //获取支配边
    vector<BasicBlock*>& getDominateEdge();
    //获取支配父节点
    BasicBlock* getParentDominateBlock();
    //获取支配子节点
    vector<BasicBlock*>& getChildDominateBlocks();
    //获取活跃变量的use集合
    unordered_set<Value*>& getUseSet();
    //获取活跃变量的def集合
    unordered_set<Value*>& getDefSet();
    //获取活跃变量的in集合
    unordered_set<Value*>& getInSet();
    //获取活跃变量的out集合
    unordered_set<Value*>& getOutSet();

    void insertPhiInstruction() override;

    /**
    * 将每个Block的phi指令转换成PC指令
    * 根据教程给出的算法
    * 首先，遍历该基本块的所有前驱，确定Ei=<Bi,B>
    * 初始化一个空的PCi指令，由于循环，所以采用PC数组记录
    * 如果Bi有多个出边，那么
    * 1. 创建新的基本块Bi'
    * 2. 将边Ei替换为<Bi,Bi'>, <Bi',B>
    * 具体的表现是，修改Bi的跳转语句，增加Bi'的跳转语句，并且更改控制流图
    * 3. 将PCi指令插入Bi'中
    * 否则，如果没有多个出边，直接加在块的最后即可，不需要新建块。
    * 随后，需要移出phi指令
    */
    void PhiToPC();

    /**
    * 将并行化PC指令改写为串行化Move指令
    * 这样才可以转换成Mips
    * 首先，获取基本块需要串行化的PC指令。
    * 根据上一个函数的措施，如果有PC，一定是倒数第二个指令是PC
    * 遍历指令
    * 每次选择目标寄存器未被依赖的pc指令改写为move
    * 如果存在环
    * 那么需要选择某个节点拆开，新建寄存器a'
    */
    void PCToMove();
};



#endif //BASICBLOCK_H
