//
// Created by PengXinyang on 24-12-12.
//

#include "../../include/optimize/GVN.h"

#include "optimize/Tool/ConstFold.h"

void GVN::runConstFold(Module *module) {
    //TODO: 现在先写一个简易版本的，直接看所有的Function下的block
    vector<Function*>& functions = module->getFunctions();
    for(const auto f : functions) {
        vector<BasicBlock*>& blocks = f->getBasicBlocks();
        for(const auto b : blocks) {
            ConstFold::handleCalculateInstruction(b);
        }
    }
}

void GVN::ConstFoldImpl(BasicBlock *basic_block) {
    ConstFold::handleCalculateInstruction(basic_block);
}
