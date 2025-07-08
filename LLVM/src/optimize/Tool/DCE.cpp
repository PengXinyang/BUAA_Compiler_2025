//
// Created by PengXinyang on 24-12-13.
//

#include "../../../include/optimize/Tool/DCE.h"

#include "value/architecture/user/Function.h"
#include "value/architecture/user/instruction/BrInstruction.h"
#include "value/architecture/user/instruction/JumpInstruction.h"
#include "value/architecture/user/instruction/ReturnInstruction.h"

void DCE::deleteBranchInstruction(BasicBlock *basic_block) {
    vector<Instruction*>& instructions = basic_block->getInstructions();
    unsigned int i = 0;
    for (i = 0; i < instructions.size(); i++) {
        Instruction* inst = instructions[i];
        if(instanceof<ReturnInstruction>(inst) || instanceof<BrInstruction>(inst) || instanceof<JumpInstruction>(inst)) {
            //记录i，从i+1的位置都要删除
            break;
        }
    }
    for(unsigned int j = instructions.size() - 1; j > i; j--) {
        instructions.pop_back();
    }
}

//用于保存哪些基本块被访问过了
static unordered_map<BasicBlock*, bool> visitBlock;
void DCE::deleteBlock(Function *function) {
    //获取第一个基本块，这个就是流图的入口。所有不可达的基本块都应当剔除
    BasicBlock* firstBlock = function->getBasicBlocks()[0];
    vector<BasicBlock*>& blocks = function->getBasicBlocks();
    visitBlock.clear();
    dfsDeleteBlock(firstBlock);
    for(auto it = blocks.begin(); it != blocks.end();) {
        if(BasicBlock* block = *it; !visitBlock[block]) {
            //说明没有访问到这个基本块
            block->setExist(false);
            it = blocks.erase(it);
        }else ++it;
    }
}

void DCE::dfsDeleteBlock(BasicBlock *basic_block) {
    visitBlock[basic_block] = true;
    Instruction* instruction = basic_block->getLastInstruction();
    if(instanceof<JumpInstruction>(instruction)) {
        //说明是直接跳转指令
        BasicBlock* des_block = dynamic_cast<JumpInstruction*>(instruction)->getDestination();
        if(des_block && !visitBlock[des_block]) {
            dfsDeleteBlock(des_block);
        }
    }else if(instanceof<BrInstruction>(instruction)) {
        //跳转两个块，都需要dfs
        BasicBlock* ifTrueBlock = dynamic_cast<BrInstruction*>(instruction)->getIfTrueBlock();
        BasicBlock* ifFalseBlock = dynamic_cast<BrInstruction*>(instruction)->getIfFalseBlock();
        if(ifTrueBlock && !visitBlock[ifTrueBlock]) {
            dfsDeleteBlock(ifTrueBlock);
        }
        if(ifFalseBlock && !visitBlock[ifFalseBlock]) {
            dfsDeleteBlock(ifFalseBlock);
        }
    }
}
