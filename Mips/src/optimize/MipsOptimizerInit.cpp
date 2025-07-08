//
// Created by PengXinyang on 24-12-10.
//

#include "optimize/MipsOptimizerInit.h"
bool MipsOptimizerInit::is_optimize = false;

void MipsOptimizerInit::init(Module* module) {
    if (is_optimize) {
        //重新活跃变量分析
        module->activeAnalysis();
        module->printActiveAnalysis();
    }
}
