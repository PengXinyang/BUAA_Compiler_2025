//
// Created by PengXinyang on 24-12-18.
//

#include "../../include/Register/RegisterDistribute.h"

#include "value/architecture/user/instruction/PhiInstruction.h"
#include "value/architecture/user/instruction/TruncInstruction.h"
#include "value/architecture/user/instruction/ZextInstruction.h"

vector<Register*> RegisterDistribute::registers;
unordered_map<Value*, Register*> RegisterDistribute::ValueToRegisterMap;
unordered_map<Register*, Value*> RegisterDistribute::RegisterToValueMap;

void RegisterDistribute::init() {
    for(auto rn = RegisterName::$t0; rn < RegisterName::$t8; rn = rn + 1) {
        registers.push_back(Register::getRegister(rn));
    }
}

void RegisterDistribute::setValueToRegisterMap(const unordered_map<Value *, Register *> &valueToRegisterMap) {
    ValueToRegisterMap = valueToRegisterMap;
}
void RegisterDistribute::setRegisterToValueMap(const unordered_map<Register *, Value *> &registerToValueMap) {
    RegisterToValueMap = registerToValueMap;
}
unordered_map<Value *, Register *> &RegisterDistribute::getValueToRegisterMap() {
    return ValueToRegisterMap;
}

Register *RegisterDistribute::distributeRegister() {
    for(auto reg:registers) {
        if(RegisterToValueMap.find(reg) == RegisterToValueMap.end() || !RegisterToValueMap[reg]) {
            //说明找到了寄存器，直接返回
            return reg;
        }
    }
    //没有找到，返回null
    return nullptr;
}

void RegisterDistribute::releaseRegister(BasicBlock *block, Instruction *instruction, unordered_map<Value *, Value *> &lastUse, unordered_set<Value *> &defined, unordered_set<Value *> &used) {
    if(!instanceof<PhiInstruction>(instruction)) {
        //不是phi指令
        for(auto op:instruction->opValueChain) {
            if(lastUse.find(op) != lastUse.end() && lastUse[op] == instruction
                && ValueToRegisterMap.find(op) != ValueToRegisterMap.end()
                && block->getOutSet().count(op) == 0) {
                RegisterToValueMap.erase(ValueToRegisterMap[op]);
                used.insert(op);
            }
        }
    }
    if(instruction->isUseful() && !instanceof<TruncInstruction>(instruction) && !instanceof<ZextInstruction>(instruction)) {
        //是有用的指令，但是不属于强制转换指令
        defined.insert(instruction);
        Register* reg = distributeRegister();
        if(reg) {
            if(RegisterToValueMap.count(reg)!=0) {
                //要从另一个map移除
                ValueToRegisterMap.erase(RegisterToValueMap[reg]);
            }
            RegisterToValueMap[reg] = instruction;
            ValueToRegisterMap[instruction] = reg;
        }
    }
}

void RegisterDistribute::buildRegisterMap(BasicBlock *block) {
    unordered_map<Register*, Value*> registerToValueMap;
    unordered_set<Value*> in_set = block->getInSet();
    for(auto [fst, snd]: RegisterToValueMap) {
        if(in_set.count(snd)==0) {
            registerToValueMap[fst] = snd;
        }
    }
    //首先，暂时移除registerToValueMap的寄存器分配，等到递归执行完后再恢复
    for(auto [fst, snd]: registerToValueMap) {
        RegisterToValueMap.erase(fst);
    }
    //然后，递归调用
    distributeInBasicBlock(block);
    //最后，恢复寄存器
    for(auto it: registerToValueMap) {
        RegisterToValueMap.insert(it);
    }
}

void RegisterDistribute::distributeInBasicBlock(BasicBlock *block) {
    unordered_map<Value*, Value*> lastUse;
    unordered_set<Value*> defined, used;
    vector<Instruction*>& instructions = block->getInstructions();
    //找到直接支配的子节点
    vector<BasicBlock*>& dominate_children = block->getChildDominateBlocks();
    for(auto instruction: instructions) {
        for(auto op: instruction->opValueChain) {
            lastUse[op] = instruction;
        }
    }
    //释放寄存器
    for(const auto instruction: instructions) {
        releaseRegister(block, instruction, lastUse, defined, used);
    }
    //遍历支配节点
    for(const auto dominate: dominate_children) {
        buildRegisterMap(dominate);
        //distributeInBasicBlock(dominate);
    }
    //对于define移除，use分配寄存器
    for(auto def: defined) {
        if(ValueToRegisterMap.find(def) != ValueToRegisterMap.end()) {
            RegisterToValueMap.erase(ValueToRegisterMap[def]);
        }
    }
    for(auto use: used) {
        if(ValueToRegisterMap.find(use) != ValueToRegisterMap.end()
            && defined.count(use)==0) {
            RegisterToValueMap[ValueToRegisterMap[use]] = use;
        }
    }
}
