//
// Created by PengXinyang on 24-12-10.
//

#include "../../../../../include/value/architecture/user/instruction/PhiInstruction.h"

#include <algorithm>

#include "value/architecture/BasicBlock.h"
#include "value/architecture/ConstValue.h"
#include "value/architecture/Param.h"
#include "value/architecture/user/GlobalVariable.h"

PhiInstruction::PhiInstruction(IRType* ir_type, const string& name, const vector<BasicBlock *>& preBlocks, int cnt)
    :Instruction(ir_type,name,"phi"){
    this->preBlocks = preBlocks;
    const int size = cnt == -1? static_cast<int>(preBlocks.size()):cnt;
    int num = size - static_cast<int>(preBlocks.size());
    addOpValue(nullptr,size);
    for(int i=0;i<num;i++) {
        this->preBlocks.push_back(nullptr);
    }
}

vector<BasicBlock *> PhiInstruction::getPreBlocks() {
    return preBlocks;
}

string PhiInstruction::toLLVM() {
    stringstream ss;
    /*
    * 示例：
    * %7 = phi i32 [1, %3], [%5, %4]
    */
    ss<<value_name<<" = "<<instructionType<<" "<<value_type->toLLVM()<<" ";
    const int size = static_cast<int>(preBlocks.size());
    for(int i=0;i<size;i++) {
        if(opValueChain[i]&&preBlocks[i]) {
            ss<<"["<<opValueChain[i]->value_name<<", %"<<preBlocks[i]->value_name<<"]";
        }
        if(i+1<size) {
            ss<<", ";
        }
    }
    return ss.str();
}

void PhiInstruction::changeValue(Value *value, BasicBlock *basic_block) {
    const auto it = find(preBlocks.begin(),preBlocks.end(),basic_block);
    const int index = it - preBlocks.begin();
    opValueChain[index] = value;
    //存在i8的可能性，所以添加值的时候就需要重新确定值的属性
    value_type = value->value_type;
    //由于初始化时，添加了null，现在需要将这个value和phi绑定
    value->addUser(this);
}

void PhiInstruction::ChangePhiToPC(vector<PCInstruction*> &pc_instructions) {
    for(int i = 0; i < opValueChain.size(); i++) {
        const auto op = opValueChain[i];
        //如果op不是常量，就需要记录源寄存器和目标寄存器，为了之后的转move指令
        if(instanceof<ConstValue>(op) && !dynamic_cast<ConstValue*>(op)->isDefined()) continue;
        pc_instructions[i]->setSrcDst(op,this);
    }
}

void PhiInstruction::addIntoUse() {
    unordered_set<Value*>& use = getBlockParent()->getUseSet();
    for(auto op:opValueChain) {
        if(instanceof<Instruction>(op) || instanceof<GlobalVariable>(op) || instanceof<Param>(op)) {
            //说明op属于指令/全局变量/参数，应当加入使用链
            use.insert(op);
        }
    }
}
