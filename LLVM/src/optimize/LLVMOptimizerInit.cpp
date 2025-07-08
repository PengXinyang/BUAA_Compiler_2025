//
// Created by PengXinyang on 24-11-28.
//

#include "../include/optimize/LLVMOptimizerInit.h"

bool LLVMOptimizerInit::is_optimize = false;
bool LLVMOptimizerInit::is_const_fold = false;

bool LLVMOptimizerInit::isOptimize() {
    return is_optimize;
}
void LLVMOptimizerInit::setIsOptimize(const bool optimize) {
    is_optimize = optimize;
}
bool LLVMOptimizerInit::isConstFold() {
    return is_const_fold;
}
void LLVMOptimizerInit::setIsConstFold(bool constFold) {
    is_const_fold = constFold;
}

void LLVMOptimizerInit::init(Module *module) {
    module->DCEBlock();
    module->buildCfgGraph();
    module->printCFG();
    module->generateDominantTree();
    module->printDominantTree();
    /*module->activeAnalysis();
    module->printActiveAnalysis();*/
}
