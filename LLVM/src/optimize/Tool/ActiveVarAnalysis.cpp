//
// Created by PengXinyang on 24-12-19.
//

#include "../../../include/optimize/Tool/ActiveVarAnalysis.h"

#include "value/architecture/user/instruction/PhiInstruction.h"

ActiveVarAnalysis::ActiveVarAnalysis(Function *function) {
    this->function = function;
}

unordered_set<Value *> &ActiveVarAnalysis::getDefVar(BasicBlock *block) {
    return def[block];
}
unordered_set<Value *> &ActiveVarAnalysis::getUseVar(BasicBlock *block) {
    return use[block];
}
unordered_set<Value *> &ActiveVarAnalysis::getInVar(BasicBlock *block) {
    return in[block];
}
unordered_set<Value *> &ActiveVarAnalysis::getOutVar(BasicBlock *block) {
    return out[block];
}

void ActiveVarAnalysis::generateUseDef() {
    use.clear();
    def.clear();
    vector<BasicBlock *>& blocks = function->getBasicBlocks();
    //遍历每个基本块的每个指令，生成即可
    for (BasicBlock *block : blocks) {
        vector<Instruction*>& instructions = block->getInstructions();
        for(const auto instruction:instructions) {
            if(instanceof<PhiInstruction>(instruction)) {
                dynamic_cast<PhiInstruction *>(instruction)->addIntoUse();
            }
        }
        for(const auto instruction:instructions) {
            instruction->generateUseDef();
        }
    }
}

void ActiveVarAnalysis::generateInOut() {
    in.clear();
    out.clear();
    vector<BasicBlock *>& blocks = function->getBasicBlocks();
    bool is_in_change = true;//标记in集合是否改变
    while(is_in_change) {
        is_in_change = false;
        //倒着遍历基本块
        for(int i = static_cast<int>(blocks.size()) - 1; i >= 0; i--) {
            BasicBlock *block = blocks[i];
            if(block->value_name=="main_label_6" || block->value_name=="main_label_3") {
                printf("debug");
            }
            unordered_set<Value *> out_set;
            //获取后继基本块，便于确定它们的in集合
            vector<BasicBlock *>& out_blocks = block->getOutBlocks();
            for(auto out_block : out_blocks) {
                unordered_set<Value*> in_set = in[out_block];
                //求这些in集合的并集
                out_set.insert(in_set.begin(), in_set.end());
            }
            out[block] = out_set;
            unordered_set<Value *> in_set;
            //按照公式，求in
            // 计算 out - def
            //unordered_set<Value*>outMinusDef;
            for(auto out_number:out[block]) {
                if(def[block].count(out_number)==0) {
                    in_set.insert(out_number);
                }
            }

            // 计算 in = use + (out - def)
            /*in_set = use[block];
            in_set.insert(outMinusDef.begin(), outMinusDef.end());*/
            for(auto use_:use[block]) {
                in_set.insert(use_);
            }
            //判断原来的in和新生成的in是否一致，如果不一致，标记为true
            if(in_set!=in[block]) {
                is_in_change = true;
            }
            if(!is_in_change) {
                printf("debug");
            }
            in[block] = in_set;
        }
    }
}

string ActiveVarAnalysis::printActiveAnalysis() const {
    ostringstream oss;
    vector<BasicBlock*>& blocks = function->getBasicBlocks();
    for (BasicBlock* block : blocks) {
        oss<<"基本块: "<<block->value_name<<"\n\t\t\t";
        oss<<"use集合: "<<"\n\t\t\t\t";
        for(const auto value: block->getUseSet()) {
            oss<<value->toLLVM()<<"\n\t\t\t\t";
        }
        oss<<"\n\t\t\tdef集合: "<<"\n\t\t\t\t";
        for(const auto value: block->getDefSet()) {
            oss<<value->toLLVM()<<"\n\t\t\t\t";
        }
        oss<<"\n\t\t\tin集合: "<<"\n\t\t\t\t";
        for(const auto value: block->getInSet()) {
            oss<<value->toLLVM()<<"\n\t\t\t\t";
        }
        oss<<"\n\t\t\tout集合: "<<"\n\t\t\t\t";
        for(const auto value: block->getOutSet()) {
            oss<<value->toLLVM()<<"\n\t\t\t\t";
        }
        oss<<"\n\t\t";
    }
    oss<<"\n\t";
    return oss.str();
}
