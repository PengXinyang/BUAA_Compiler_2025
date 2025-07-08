//
// Created by PengXinyang on 24-12-10.
//

#include "../include/ConfigOptimizer.h"

#include "optimize/LLVMOptimizerInit.h"
#include "optimize/MipsOptimizerInit.h"

bool ConfigOptimizer::config_optimize = false;
bool ConfigOptimizer::isConfigOptimize() {
    return config_optimize;
}
void ConfigOptimizer::setConfigOptimize(bool is_optimize) {
    config_optimize = is_optimize;
}

void ConfigOptimizer::initConfigOptimizer() {
    if (!config_optimize) {
        //全部不开启优化
        LLVMOptimizerInit::setIsOptimize(false);
        MipsOptimizerInit::setIsOptimize(false);
    }
    //否则，根据情况打开
    LLVMOptimizerInit::setIsOptimize(true);
    MipsOptimizerInit::setIsOptimize(true);
}
