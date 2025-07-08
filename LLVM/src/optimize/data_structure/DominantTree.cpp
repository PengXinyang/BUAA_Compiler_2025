//
// Created by PengXinyang on 24-12-13.
//

#include "../../../include/optimize/data_structure/DominantTree.h"
DominantTree::DominantTree(Function *function) {
    this->function = function;
    //initDominateBlocks();
}
unordered_map<BasicBlock *, vector<BasicBlock *> >& DominantTree::getDominateBlocks() {
    return dominateBlocks;
}
unordered_map<BasicBlock *, vector<BasicBlock *> > &DominantTree::getDominateEdge() {
    return dominateEdge;
}
unordered_map<BasicBlock *, BasicBlock *> &DominantTree::getParentBlock() {
    return parentBlock;
}
unordered_map<BasicBlock *, vector<BasicBlock *> > &DominantTree::getChildBlock() {
    return childBlock;
}


void DominantTree::initDominateBlocks() {
    vector<BasicBlock*>& blocks = function->getBasicBlocks();
    for (BasicBlock* block : blocks) {
        dominateBlocks[block].push_back(block);
    }
}

void DominantTree::generateDominateBlocks() {
    vector<BasicBlock*>& blocks = function->getBasicBlocks();
    BasicBlock* entryBlock = blocks[0];
    for (BasicBlock* block : blocks) {
        unordered_set<BasicBlock*> noDominateBlocks;
        dfsDominateBlocks(entryBlock,block,noDominateBlocks);
        vector<BasicBlock*> dominateBlocks;
        for(BasicBlock* basic_block:blocks) {
            if(!noDominateBlocks.count(basic_block)) {
                dominateBlocks.push_back(basic_block);
            }
        }
        this->dominateBlocks[block] = dominateBlocks;
    }
}

void DominantTree::dfsDominateBlocks(BasicBlock *entryBlock, BasicBlock *block, unordered_set<BasicBlock *> &noDominateSet) {
    if(entryBlock == block) {
        return;
    }
    noDominateSet.insert(entryBlock);
    vector<BasicBlock*>& outBlocks = entryBlock->getOutBlocks();
    for (BasicBlock* outBlock : outBlocks) {
        if(noDominateSet.find(outBlock) == noDominateSet.end()) {
            //说明集合里没有这个基本块，可以继续递归
            dfsDominateBlocks(outBlock, block, noDominateSet);
        }
    }
}

void DominantTree::generateDominantTree() {
    vector<BasicBlock*>& functionBlocks = function->getBasicBlocks();
    for (BasicBlock* functionBlock : functionBlocks) {
        vector<BasicBlock*>& dominate_Blocks = functionBlock->getDominateBlocks();
        for (BasicBlock* dominate_Block : dominate_Blocks) {
            //这里需要判断两个基本块是否直接支配
            /**
            * 我们希望，让functionBlock直接支配dominate_block
            * 只要支配+不相等，那么就是严格支配
            * 那么就需要判断，functionBlock支配的节点中，有没有支配dominate_Block的节点
            * 如果有，那么说明functionBlock不是直接支配dominate_block
            */
            bool flag = functionBlock != dominate_Block;
            if(flag) {
                for(const auto mid_block: dominate_Blocks) {
                    if(mid_block!=dominate_Block && mid_block!= functionBlock) {
                        if(vector<BasicBlock*>& mid_dominate_Blocks = mid_block->getDominateBlocks();
                            count(mid_dominate_Blocks.begin(), mid_dominate_Blocks.end(), dominate_Block) != 0) {
                            flag = false;
                            break;
                        }
                    }
                }
            }
            //如果flag是true，说明确实严格支配
            if(flag) {
                //需要添加双向边，也就是添加支配树的父节点和子节点
                //首先必须明确，在这里functionBlock是父节点，dominate_Block是子节点
                parentBlock[dominate_Block] = functionBlock;
                childBlock[functionBlock].push_back(dominate_Block);
            }
        }
    }
}

void DominantTree::generateDominateEdge() {
    unordered_map<BasicBlock*, vector<BasicBlock*>>& CfgOut = function->getCfgGraph()->getOutBlocks();
    for(const auto&[fst, snd]: CfgOut) {
        BasicBlock* entryBlock = fst;
        vector<BasicBlock *> dominate_Blocks = snd;
        for(auto dominate_Block: dominate_Blocks) {
            //现在开始，遍历流图所有的边，相当于for ⟨a,b⟩ ∈ CFG 图的边集
            BasicBlock* x = entryBlock;
            //我们现在需要找一个条件，判断x不严格支配b
            //说白了就两种可能性：要么x和b相等，要么x不支配b(x的支配集不存在b)
            //TODO:出现严重问题：可能支配树出现了环?
            //20和28号基本块成环
            while(x!=nullptr && (x == dominate_Block ||
                count(x->getDominateBlocks().begin(), x->getDominateBlocks().end(), dominate_Block) == 0)) {
                //将b纳入
                dominateEdge[x].push_back(dominate_Block);
                x = x->getParentDominateBlock();
            }
        }
    }
}

string DominantTree::printDominantTree() {
    ostringstream oss;
    vector<BasicBlock*>& blocks = function->getBasicBlocks();
    for (BasicBlock* block : blocks) {
        oss<<"基本块: "<<block->value_name<<"\n\t\t\t";
        oss<<"支配集合: "<<"\n\t\t\t\t";
        for(auto basic_block: block->getDominateBlocks()) {
            oss<<basic_block->value_name<<"\n\t\t\t\t";
        }
        oss<<"\n\t\t\t支配边: "<<"\n\t\t\t\t";
        for(auto basic_block: block->getDominateEdge()) {
            oss<<basic_block->value_name<<"\n\t\t\t\t";
        }
        oss<<"\n\t\t\t支配树父节点: "<<"\n\t\t\t\t";
        if(parentBlock[block]) {
            oss<<parentBlock[block]->value_name<<"\n\t\t\t\t";
        }else oss<<"没有父节点"<<"\n\t\t\t\t";
        oss<<"\n\t\t\t支配树子节点: "<<"\n\t\t\t\t";
        for(auto basic_block: block->getChildDominateBlocks()) {
            oss<<basic_block->value_name<<"\n\t\t\t\t";
        }
        oss<<"\n\t\t";
    }
    oss<<"\n\t";
    return oss.str();
}
