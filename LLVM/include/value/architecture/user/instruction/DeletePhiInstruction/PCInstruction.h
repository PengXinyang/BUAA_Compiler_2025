//
// Created by PengXinyang on 24-12-18.
//

#ifndef PCINSTRUCTION_H
#define PCINSTRUCTION_H
#include "value/architecture/user/Instruction.h"

/**
* 这里的PC指令指的是Parallel copy, 仅用于消除phi
* 实际上这个指令在LLVM IR中不存在，并且也没有对应的Mips
* 只是方便消除phi
* 由于原来的phi可能作为操作数被别的量使用了
* 所以这里PC也需要采用Instruction类
* pc: a1 → b1, a2 → b2, ..., an → bn
* 表示同时将 a1 赋值给 b1，a2 赋值给 b2，依此类推。
*/
class PCInstruction : public Instruction{
private:
    /**
    * 记录源寄存器, 为了方便串行化move指令的构建
    */
    vector<Value*> source;
    /**
    * 记录目标寄存器, 为了方便串行化move指令的构建
    */
    vector<Value*> destination;
public:
    PCInstruction() = default;
    explicit PCInstruction(const string& name);
    vector<Value*>& getSource();//获取源寄存器
    vector<Value*>& getDestination();//获取目标寄存器
    /**
    * 添加源寄存器和目标寄存器
    * @param src 源
    * @param dst 目标
    */
    void setSrcDst(Value* src, Value* dst);
};



#endif //PCINSTRUCTION_H
