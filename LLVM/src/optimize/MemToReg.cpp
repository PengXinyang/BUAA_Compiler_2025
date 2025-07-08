//
// Created by PengXinyang on 24-12-10.
//

#include "../../include/optimize/MemToReg.h"

#include "optimize/LLVMOptimizerInit.h"
#include "type/IRName.h"
#include "type/irType/IRInt.h"
#include "value/architecture/ConstValue.h"
#include "value/architecture/user/Instruction.h"
#include "value/architecture/user/instruction/LoadInstruction.h"
#include "value/architecture/user/instruction/PhiInstruction.h"
#include "value/architecture/user/instruction/StoreInstruction.h"
Module* MemToReg::module;
BasicBlock* MemToReg::firstBlock;
vector<Instruction*> MemToReg::loadInstructions;
vector<Instruction*> MemToReg::storeInstructions;
vector<BasicBlock*> MemToReg::useBlocks;
vector<BasicBlock*> MemToReg::defineBlocks;
vector<Value*> MemToReg::phiValueVector;
Instruction* MemToReg::nowAllocaInstruction;

BasicBlock *MemToReg::getFirstBlock() {
    return firstBlock;
}
void MemToReg::setFirstBlock(BasicBlock *first_block) {
    firstBlock = first_block;
}

void MemToReg::Mem2Reg(Module *module) {
    MemToReg::module = module;
    LLVMOptimizerInit::init(module);
    vector<Function*>& functions = module->getFunctions();
    for(const auto f : functions) {
        f->insertPhiInstruction();
    }
}

void MemToReg::resetByInstruction(Instruction *instruction) {
    //清空上述的数组，进行重置
    loadInstructions = vector<Instruction*>();
    storeInstructions = vector<Instruction*>();
    useBlocks = vector<BasicBlock*>();
    defineBlocks = vector<BasicBlock*>();
    phiValueVector = vector<Value*>();
    nowAllocaInstruction = instruction;
    for(const Use* use: instruction->getUseChain()) {
        User* user = use->getUser();
        if(!instanceof<Instruction>(user)) continue;
        //查看哪些指令使用了alloca，分别将它们加入到对应的数组
        auto* inst = dynamic_cast<Instruction*>(user);
        BasicBlock* block = inst->getBlockParent();
        if(instanceof<LoadInstruction>(inst) && block->isExist()) {
            //这个基本块没有被消除，并且指令是load
            loadInstructions.push_back(inst);
            if(count(defineBlocks.begin(),defineBlocks.end(),block) == 0) {
                useBlocks.push_back(block);
            }
        }else if(instanceof<StoreInstruction>(inst) && block->isExist()) {
            storeInstructions.push_back(inst);
            if(count(defineBlocks.begin(),defineBlocks.end(),block) == 0) {
                defineBlocks.push_back(block);
            }
        }
    }
}

void MemToReg::insertPhiInstruction() {
    vector<BasicBlock*> F;
    vector<BasicBlock*> W;
    W.reserve(defineBlocks.size());
    for(auto d: defineBlocks) {
        W.push_back(d);
    }
    while(!W.empty()) {
        BasicBlock* x =W.back();
        W.pop_back();
        vector<BasicBlock*>& DFX = x->getDominateEdge();
        for(auto y: DFX) {
            if(count(F.begin(),F.end(),y) == 0) {
                //首先在F加入
                F.push_back(y);
                //在基本块Y的入口处插入空的ϕ函数
                auto phiInstruction = new PhiInstruction(
                    new IRInt(),
                    IRName::getLocalVariableName(y->getFunctionParent()),
                    y->getInBlocks());
                y->addInstruction(phiInstruction,0);
                loadInstructions.push_back(phiInstruction);
                storeInstructions.push_back(phiInstruction);
                if(count(defineBlocks.begin(),defineBlocks.end(),y) == 0) {
                    //y不在定义块中，要把y添加到W中
                    W.push_back(y);
                }
            }
        }
    }
}

void MemToReg::renameVar(BasicBlock *basic_block) {
    int cnt = removeInstruction(basic_block);
    //获取后继的block
    //注意，不能直接获取支配树的后续，因为存在一种情况：
    //1->2->3, 1->3, 这种情况1不是直接支配3，但是3确实是1的后继
    //在基本块3中，很可能有phi指令，所以需要赋值，那么应当获得的是流图的后继
    vector<BasicBlock*> outBlocks = basic_block->getOutBlocks();
    for(const auto outBlock: outBlocks) {
        vector<Instruction*> outInsts = outBlock->getInstructions();
        //TODO: 可能会有多个phi指令
        for(auto inst: outInsts) {
            if(instanceof<PhiInstruction>(inst)&&
                count(loadInstructions.begin(),loadInstructions.end(),inst) > 0) {
                Value* value = nullptr;
                if(phiValueVector.empty()) {
                    //理论上来说，这属于没有定义? 为了保证程序不RE，生成一个常数
                    value = new ConstValue(new IRInt(),"0", false);
                }
                else value = phiValueVector.back();
                auto* phi = dynamic_cast<PhiInstruction*>(inst);
                phi->changeValue(value, basic_block);
            }
        }
    }
    //获取支配树的子节点，也进行遍历
    vector<BasicBlock*>& childDominateBlocks = basic_block->getChildDominateBlocks();
    for(const auto childDominateBlock: childDominateBlocks) {
        renameVar(childDominateBlock);
    }
    //弹出
    for(int i = 0; i< cnt; i++) {
        phiValueVector.pop_back();
    }
}

int MemToReg::removeInstruction(BasicBlock *basic_block) {
    int cnt = 0;
    vector<Instruction*>& instructions = basic_block->getInstructions();
    for(auto it = instructions.begin(); it != instructions.end();) {
        Instruction* inst = *it;
        if(inst==nowAllocaInstruction) {
            //是alloca指令，直接删除
            const auto it_new = instructions.erase(it);
            inst->dropOpValue();
            it = it_new;
        }
        else if(instanceof<StoreInstruction>(inst) &&
            count(storeInstructions.begin(),storeInstructions.end(),inst) != 0) {
            //是store指令，压栈
            //指令数目+1
            cnt++;
            //取出存的value
            Value* value = inst->opValueChain[0];
            //存入phi使用的value中
            phiValueVector.push_back(value);
            //删除store指令
            const auto it_new = instructions.erase(it);
            inst->dropOpValue();
            it = it_new;
        }else if(instanceof<LoadInstruction>(inst) &&
            count(loadInstructions.begin(),loadInstructions.end(),inst) != 0) {
            //是load指令，直接赋成值，同时需要维护对应的定义使用链
            Value* value = nullptr;
            if(phiValueVector.empty()) {
                //理论上来说，这属于没有定义? 为了保证程序不RE，生成一个常数
                value = new ConstValue(new IRInt(),"0", false);
            }
            else value = phiValueVector.back();
            inst->replaceAllUser(value);
            it = instructions.erase(it);
        }else if(instanceof<PhiInstruction>(inst) &&
            count(storeInstructions.begin(),storeInstructions.end(),inst) != 0) {
            cnt++;
            phiValueVector.push_back(inst);
            ++it;
        }
        else ++it;
    }
    return cnt;
}
