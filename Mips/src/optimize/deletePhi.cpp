//
// Created by PengXinyang on 24-12-18.
//

#include "../../include/optimize/deletePhi.h"

#include "Register/RegisterDistribute.h"
#include "type/IRName.h"
#include "value/architecture/ConstValue.h"
Module* deletePhi::module;
void deletePhi::DeletePhi(Module* module) {
    deletePhi::module = module;
    //module->activeAnalysis();
    vector<Function*>& functions = module->getFunctions();
    //首先，需要分配寄存器
    RegisterDistribute::init();
    for(const auto f : functions) {
        f->registerDistribute();
    }
    //随后，消除phi指令
    for(const auto f : functions) {
        f->deletePhiInstruction();
    }
    //输出debug语句
    module->printRegisterDistribute();
}


vector<PhiMoveInstruction *> deletePhi::generateMoveTemp(Function *function, vector<PhiMoveInstruction *> &phi_move_instructions) {
    vector<PhiMoveInstruction *> cycle_instructions = generateMoveCycle(function, phi_move_instructions);
    vector<PhiMoveInstruction *> same_register_move_instructions = generateMoveSameRegister(function, phi_move_instructions);
    for(auto phi_move_instruction : same_register_move_instructions) {
        cycle_instructions.push_back(phi_move_instruction);
    }
    return cycle_instructions;
}

vector<PhiMoveInstruction *> deletePhi::generateMoveCycle(Function *function, vector<PhiMoveInstruction *> &phi_move_instructions) {
    vector<PhiMoveInstruction *> cycle_instructions;
    unordered_map<Value*, bool> visit;
    for(int i = 0; i < phi_move_instructions.size(); i++) {
        Value* dest = phi_move_instructions[i]->getDestination();
        if(!instanceof<ConstValue>(dest) && !visit[dest]) {
            visit[dest] = true;
            bool is_cycle = false;//标记有没有循环
            for(int j = i+1;j < phi_move_instructions.size();j++) {
                if(phi_move_instructions[j]->getSource() == dest) {
                    is_cycle = true;
                    break;
                }
            }
            if(is_cycle) {
                //说明存在循环，需要破除, 添加临时寄存器
                auto* temp = new Value(dest->value_type,"temp_"+dest->value_name);
                for(const auto phi_move_instruction : phi_move_instructions) {
                    if(phi_move_instruction->getSource() == dest) {
                        phi_move_instruction->setSource(temp);
                    }
                }
                auto* new_phi_move_instruction = new PhiMoveInstruction(
                    IRName::getLocalVariableName(function),dest,temp);
                //将上面这条转移到temp_value的语句移动进cycle_instructions最开始的位置
                cycle_instructions.insert(cycle_instructions.begin(),new_phi_move_instruction);
            }
        }
    }
    return cycle_instructions;
}

vector<PhiMoveInstruction *> deletePhi::generateMoveSameRegister(Function *function, vector<PhiMoveInstruction *> &phi_move_instructions) {
    vector<PhiMoveInstruction *> SameRegister_instructions;
    unordered_map<Value*, bool> visit;
    auto valueRegisterMap = function->getValueRegisterMap();
    for(int i = phi_move_instructions.size()-1; i >= 0; i--) {
        Value* src = phi_move_instructions[i]->getSource();
        if(!instanceof<ConstValue>(src) && !visit[src]) {
            visit[src] = true;
            bool is_same_register = false;
            for(int j = 0;j < i;j++) {
                if(valueRegisterMap[src] && valueRegisterMap[src] == valueRegisterMap[phi_move_instructions[j]->getDestination()]) {
                    //说明前面存在指令，目标寄存器和该寄存器重合，需要分配临时寄存器
                    is_same_register = true;
                    break;
                }
            }
            if(is_same_register) {
                auto* temp = new Value(src->value_type,"temp_"+src->value_name);
                for(const auto phi_move_instruction : phi_move_instructions) {
                    if(phi_move_instruction->getSource() == src) {
                        phi_move_instruction->setSource(temp);
                    }
                }
                auto* new_phi_move_instruction = new PhiMoveInstruction(
                    IRName::getLocalVariableName(function),src,temp);
                SameRegister_instructions.insert(SameRegister_instructions.begin(),new_phi_move_instruction);
            }
        }
    }
    return SameRegister_instructions;
}
