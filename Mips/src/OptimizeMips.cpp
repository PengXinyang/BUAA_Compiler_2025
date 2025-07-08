//
// Created by PengXinyang on 24-12-18.
//

#include "../include/OptimizeMips.h"

#include "GenerateIR.h"
#include "optimize/deletePhi.h"
#include "optimize/MipsOptimizerInit.h"
OptimizeMips* OptimizeMips::instance;
OptimizeMips::OptimizeMips() {
    module = GenerateIR::getInstance()->getModule();
}

OptimizeMips *OptimizeMips::getInstance() {
    if (!instance) {
        instance = new OptimizeMips();
    }
    return instance;
}

void OptimizeMips::runOptimizeMips() {
    MipsOptimizerInit::init(module);
    //消除phi函数
    deletePhi::DeletePhi(module);

}
