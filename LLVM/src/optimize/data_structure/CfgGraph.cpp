//
// Created by PengXinyang on 24-12-12.
//

#include "optimize/data_structure/CfgGraph.h"

#include "value/architecture/user/instruction/BrInstruction.h"
#include "value/architecture/user/instruction/JumpInstruction.h"
#include "value/architecture/user/instruction/ReturnInstruction.h"

CfgGraph::CfgGraph(Function *function) {
    this->function = function;
}
unordered_map<BasicBlock *, vector<BasicBlock *> > &CfgGraph::getInBlocks() {
    return inBlocks;
}
unordered_map<BasicBlock *, vector<BasicBlock *> > &CfgGraph::getOutBlocks() {
    return outBlocks;
}

void CfgGraph::buildCFG() {
    vector<BasicBlock *>& blocks = function->getBasicBlocks();
    for (BasicBlock *block : blocks) {
        Instruction* last_instruction = block->getLastInstruction();
        Instruction* penu_instruction = block->getPenuInstruction();
        //看最后一条指令是不是跳转，只有跳转是区分基本块的标准
        //并且跳转的语句前一句不能是return指令，否则到不了跳转指令，自然也到不了跳转的基本块
        if(instanceof<JumpInstruction>(last_instruction) && (!penu_instruction || !instanceof<ReturnInstruction>(penu_instruction))) {
            //直接跳转指令
            addEdge(block,
                dynamic_cast<JumpInstruction*>(last_instruction)->getDestination());
        }else if(instanceof<BrInstruction>(last_instruction)&& !instanceof<ReturnInstruction>(penu_instruction)) {
            //是条件跳转指令，流图要建立向if内的基本块和外面的基本块
            addEdge(block,
                dynamic_cast<BrInstruction*>(last_instruction)->getIfTrueBlock());
            addEdge(block,
                dynamic_cast<BrInstruction*>(last_instruction)->getIfFalseBlock());
        }
    }
}

void CfgGraph::insertBlock(BasicBlock *from_block, BasicBlock *to_block, BasicBlock *mid_block) {
    //to_block在outBlock的位置
    int to_block_out = find(outBlocks[from_block].begin(),outBlocks[from_block].end(),to_block) - outBlocks[from_block].begin();
    //from_block在inBlock的位置
    int from_block_in = find(inBlocks[to_block].begin(),inBlocks[to_block].end(),from_block) - inBlocks[to_block].begin();
    //在对应位置添加中间块
    addInBlock(to_block,mid_block,from_block_in);
    addOutBlock(from_block,mid_block,to_block_out);
    //移除原本的基本块
    to_block_out = find(outBlocks[from_block].begin(),outBlocks[from_block].end(),to_block) - outBlocks[from_block].begin();
    from_block_in = find(inBlocks[to_block].begin(),inBlocks[to_block].end(),from_block) - inBlocks[to_block].begin();
    outBlocks[from_block].erase(outBlocks[from_block].begin()+to_block_out);
    inBlocks[to_block].erase(inBlocks[to_block].begin()+from_block_in);
    //将midBlock与两个基本块连接起来
    addOutBlock(from_block,mid_block);
    addInBlock(to_block,mid_block);
}

void CfgGraph::mergeBlock(BasicBlock *from_block, BasicBlock *to_block) {
    // 从 preBasicBlock 的 outBlocks 中移除 basicBlock
    auto& fromOutBlocks = from_block->getOutBlocks();
    auto it = std::find(fromOutBlocks.begin(), fromOutBlocks.end(), to_block);
    if (it != fromOutBlocks.end()) {
        fromOutBlocks.erase(it);
    }

    // 遍历 basicBlock 的 outBlocks
    auto& basicOutBlocks = to_block->getOutBlocks();
    for (auto* outBasicBlock : basicOutBlocks) {
        // 添加到 preBasicBlock 的 outBlocks 中
        fromOutBlocks.push_back(outBasicBlock);

        // 从 outBasicBlock 的 inBlocks 中移除 basicBlock
        auto& outIndBlocks = outBasicBlock->getInBlocks();
        auto indIt = std::find(outIndBlocks.begin(), outIndBlocks.end(), to_block);
        if (indIt != outIndBlocks.end()) {
            outIndBlocks.erase(indIt);
        }

        // 添加 from_block 到 outBasicBlock 的 inBlocks 中
        outIndBlocks.push_back(from_block);
    }
}


void CfgGraph::addEdge(BasicBlock *from_block, BasicBlock *to_block) {
    addInBlock(to_block, from_block);//添加前序
    addOutBlock(from_block, to_block);//添加后序
}

void CfgGraph::addInBlock(BasicBlock *to_block, BasicBlock *from_block, const int index) {
    if(index != -1 && index <= inBlocks[to_block].size()) {
        //说明输入了参数，要在指定位置添加block
        // 获取迭代器指向插入位置
        auto& vec = inBlocks[to_block];
        vec.insert(vec.begin() + index, from_block); // 在指定位置插入
    }else {
        //在最后添加
        inBlocks[to_block].push_back(from_block);
    }
}

void CfgGraph::addOutBlock(BasicBlock *from_block, BasicBlock *to_block, const int index) {
    if(index != -1 && index <= outBlocks[from_block].size()) {
        auto& vec = outBlocks[from_block];
        vec.insert(vec.begin() + index, to_block);
    }else {
        outBlocks[from_block].push_back(to_block);
    }
}

string CfgGraph::printCFG() {
    ostringstream oss;
    oss<<"函数: "<<function->value_name<<"\n\t\t";
    vector<BasicBlock*> blocks = function->getBasicBlocks();
    for(auto* block : blocks) {
        oss<<"基本块: "<<block->value_name<<"\n\t\t\t";
        oss<<"inBlocks: \n\t\t\t\t";
        for(const auto* inBlock : block->getInBlocks()) {
            oss<<inBlock->value_name<<"\n\t\t\t\t";
        }
        oss<<"\n\t\t\t";
        oss<<"outBlocks: \n\t\t\t\t";
        for(const auto* outBlock : block->getOutBlocks()) {
            oss<<outBlock->value_name<<"\n\t\t\t\t";
        }
        oss<<"\n\t\t";
    }
    return oss.str();
}
