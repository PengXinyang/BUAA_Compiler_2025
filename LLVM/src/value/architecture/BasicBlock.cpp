//
// Created by PengXinyang on 24-11-23.
//

#include "../../../include/value/architecture/BasicBlock.h"

#include <sstream>
#include <algorithm>

#include "optimize/deletePhi.h"
#include "optimize/LLVMOptimizerInit.h"
#include "optimize/MemToReg.h"
#include "optimize/Tool/ActiveVarAnalysis.h"
#include "optimize/Tool/DCE.h"
#include "structure/text/MipsBlock.h"
#include "type/IRName.h"
#include "type/irType/IRBlock.h"
#include "type/irType/IRChar.h"
#include "type/irType/IRInt.h"
#include "value/architecture/data_structure/Loop.h"
#include "value/architecture/user/Instruction.h"
#include "value/architecture/user/instruction/AllocaInstruction.h"
#include "value/architecture/user/instruction/BrInstruction.h"
#include "value/architecture/user/instruction/JumpInstruction.h"
#include "value/architecture/user/instruction/PhiInstruction.h"
#include "value/architecture/user/instruction/DeletePhiInstruction/PCInstruction.h"
#include "value/architecture/user/instruction/DeletePhiInstruction/PhiMoveInstruction.h"

BasicBlock::BasicBlock(const string& name) :Value(new IRBlock("BasicBlock"),name){
    instructions = vector<Instruction*>();
    is_exist = true;
    if(!LLVMOptimizerInit::isOptimize()) {
        IRName::addBasicBlock(this);
    }
}
void BasicBlock::setExist(bool exist) {
    is_exist = exist;
}
bool BasicBlock::isExist() const {
    return is_exist;
}
Loop *BasicBlock::getLoopParent() const {
    return loopParent;
}
void BasicBlock::setLoopParent(Loop *loop) {
    loopParent = loop;
}
int BasicBlock::getLoopDepth() const {
    return loopParent->getLoopDepth();
}

void BasicBlock::addInstruction(Instruction *instruction, int index) {
    if(index>=0 && index<instructions.size()) {
        instructions.insert(instructions.begin()+index, instruction);
    }else {
        instructions.push_back(instruction);
    }
    instruction->setBlockParent(this);
}
void BasicBlock::setInstructions(const vector<Instruction *> &instructions) {
    this->instructions = instructions;
}

vector<Instruction *>& BasicBlock::getInstructions() {
    return instructions;
}

Function *BasicBlock::getFunctionParent() const {
    return functionParent;
}
void BasicBlock::setFunctionParent(Function *function) {
    functionParent = function;
}

string BasicBlock::toLLVM() {
    //基本块的字符串输出
    ostringstream os;
    //首先，将块的名字输出，尤其是在for循环和if-else部分，块号需要输出
    //至于块的名字，计划采用前缀+块号，优化前块号采用全局符号表号
    os<<value_name<<":\n\t";
    const int size = static_cast<int>(instructions.size());
    for(int i = 0; i < size; i++) {
        os<<instructions[i]->toLLVM();
        if(i!=size-1) os<<"\n\t";
    }
    return os.str();
}

void BasicBlock::generateMIPS() {
    new MipsBlock(value_name);
    for(const auto instruction : instructions) {
        instruction->generateMIPS();
    }
}

//------------------以下用于代码优化-----------------//
void BasicBlock::DCEBlockInstruction() {
    DCE::deleteBranchInstruction(this);
}

vector<BasicBlock *> &BasicBlock::getInBlocks() {
    return functionParent->getCfgGraph()->getInBlocks()[this];
}

vector<BasicBlock *> &BasicBlock::getOutBlocks() {
    return functionParent->getCfgGraph()->getOutBlocks()[this];
}

vector<BasicBlock *> &BasicBlock::getDominateBlocks() {
    return functionParent->getDominantTree()->getDominateBlocks()[this];
}

vector<BasicBlock *> &BasicBlock::getDominateEdge() {
    return functionParent->getDominantTree()->getDominateEdge()[this];
}
BasicBlock *BasicBlock::getParentDominateBlock() {
    return functionParent->getDominantTree()->getParentBlock()[this];
}
vector<BasicBlock *> &BasicBlock::getChildDominateBlocks() {
    return functionParent->getDominantTree()->getChildBlock()[this];
}
unordered_set<Value *> &BasicBlock::getUseSet() {
    return functionParent->getActiveVarAnalysis()->getUseVar(this);
}
unordered_set<Value *> &BasicBlock::getDefSet() {
    return functionParent->getActiveVarAnalysis()->getDefVar(this);
}
unordered_set<Value *> &BasicBlock::getInSet() {
    return functionParent->getActiveVarAnalysis()->getInVar(this);
}
unordered_set<Value *> &BasicBlock::getOutSet() {
    return functionParent->getActiveVarAnalysis()->getOutVar(this);
}


void BasicBlock::insertPhiInstruction() {
    auto new_instructions = vector(instructions);
    for(auto instruction : new_instructions) {
        if(instanceof<AllocaInstruction>(instruction)) {
            auto* alloca = dynamic_cast<AllocaInstruction*>(instruction);
            if(instanceof<IRPointer>(alloca->value_type)) {
                IRType* ir_type = dynamic_cast<IRPointer*>(alloca->value_type)->ir_type();
                if(instanceof<IRInt>(ir_type) || instanceof<IRChar>(ir_type)) {
                    //说明这个alloca分配的是int或char类型，要改为phi指令
                    instruction->insertPhiInstruction();
                    MemToReg::renameVar();
                }
            }
        }
    }
}

void BasicBlock::PhiToPC() {
    //首先看这个基本块有没有phi，没有phi就不需要做
    if(instanceof<PhiInstruction>(instructions[0])) {
        //说明有phi
        vector<PCInstruction*> PCInstructions;//记录PC指令
        //根据前驱基本块找入边
        vector<BasicBlock *> inBlocks = getInBlocks();
        for(const auto inBlock : inBlocks) {
            auto* pc_instruction = new PCInstruction(IRName::getLocalVariableName(functionParent));
            PCInstructions.push_back(pc_instruction);
            //如果Bi有多条出边，进入分叉
            if(inBlock->getOutBlocks().size()>1) {
                //创建新的基本块Bi'
                auto* Bi_new = new BasicBlock(IRName::getBlockName(functionParent));
                //首先向functionParent中添加这个基本块
                //因为要插在两个基本块中间，所以要确定id
                vector<BasicBlock*> basic_blocks = functionParent->getBasicBlocks();
                //因为要插在当前基本块之前，所以是查“this”的位置
                const int index = find(basic_blocks.begin(), basic_blocks.end(), this) - basic_blocks.begin();
                functionParent->addBasicBlock(Bi_new, index);
                //向这个基本块添加PC指令
                Bi_new->addInstruction(pc_instruction);
                //重新设置跳转语句，注意，因为Bi有多条出边，所以不可能是直接跳转语句，一定是BrInstruction
                auto* last_instruction = inBlock->getLastInstruction();
                if(instanceof<BrInstruction>(last_instruction)) {
                    auto* br_instruction = dynamic_cast<BrInstruction*>(last_instruction);
                    //根据跳转的位置，将跳转块设置为Bi_new
                    if(this == br_instruction->getIfTrueBlock()) {
                        //说明原本是true的时候跳转到这个块
                        br_instruction->setIfTrueBlock(Bi_new);
                    }else if(this == br_instruction->getIfFalseBlock()) {
                        //说明是false跳转过来的
                        br_instruction->setifFalseBlock(Bi_new);
                    }
                    //添加增加的块到这个块的直接跳转语句
                    Bi_new->addInstruction(new JumpInstruction(this));
                }
                //需要修改控制流图
                functionParent->getCfgGraph()->insertBlock(inBlock,this,Bi_new);
            }else {
                //将指令加入末尾，当然肯定要加在跳转之前，否则运行不到，没有意义
                inBlock->addInstruction(pc_instruction,inBlock->getInstructions().size()-1);
            }
        }
        //现在PC已经插入，需要删除phi指令
        for(auto it = instructions.begin(); it != instructions.end();) {
            if(Instruction *instruction = *it; instanceof<PhiInstruction>(instruction)) {
                auto* phi_instruction = dynamic_cast<PhiInstruction*>(instruction);
                phi_instruction->ChangePhiToPC(PCInstructions);
                phi_instruction->dropOpValue();
                //移出it
                it = instructions.erase(it);
            }else ++it;
        }
    }
}

void BasicBlock::PCToMove() {
    //首先，判断倒数第二个是不是PC指令
    if(instructions.size()>=2 && instanceof<PCInstruction>(instructions[instructions.size()-2])) {
        auto* pc_instruction = dynamic_cast<PCInstruction*>(instructions[instructions.size()-2]);
        //需要移出指令
        instructions.erase(instructions.begin()+instructions.size()-2);
        vector<PhiMoveInstruction*> PhiMoveInstructions;
        vector<Value*> pc_source = pc_instruction->getSource();
        vector<Value*> pc_dest = pc_instruction->getDestination();
        for(int i = 0;i<pc_source.size();i++) {
            auto* phi_move_instruction = new PhiMoveInstruction(
                IRName::getLocalVariableName(functionParent),
                pc_source[i],
                pc_dest[i]);
            PhiMoveInstructions.push_back(phi_move_instruction);
        }
        //现在需要添加拆除循环, 添加新的临时寄存器用于move
        auto tempMove = deletePhi::generateMoveTemp(functionParent,PhiMoveInstructions);
        for(auto temp: tempMove) {
            //在头部插入?
            PhiMoveInstructions.insert(PhiMoveInstructions.begin(),temp);
        }

        //最终，要在倒数第二个位置依次插入这些move指令
        for(const auto temp: PhiMoveInstructions) {
            instructions.insert(instructions.end()-1,temp);
        }
    }
}
