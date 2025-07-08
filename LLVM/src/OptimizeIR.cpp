//
// Created by PengXinyang on 24-12-12.
//

#include "../include/OptimizeIR.h"

#include "GenerateIR.h"
#include "optimize/GVN.h"
#include "optimize/LLVMOptimizerInit.h"
#include "optimize/MemToReg.h"
OptimizeIR* OptimizeIR::instance;
OptimizeIR::OptimizeIR() {
    module = GenerateIR::getInstance()->getModule();
}
OptimizeIR* OptimizeIR::getInstance() {
    if (!instance) {
        instance = new OptimizeIR();
    }
    return instance;
}

void OptimizeIR::runOptimizeIR() {
    MemToReg::Mem2Reg(module);
    if(LLVMOptimizerInit::isConstFold()) {
        //说明要运行常量折叠
        GVN::runConstFold(module);
    }
}
