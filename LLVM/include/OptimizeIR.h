//
// Created by PengXinyang on 24-12-12.
//

#ifndef OPTIMIZEIR_H
#define OPTIMIZEIR_H
#include "value/architecture/Module.h"

/**
* 整体优化模块
*/
class OptimizeIR {
private:
    static OptimizeIR* instance;
    Module* module = nullptr;
    OptimizeIR();
public:
    static OptimizeIR* getInstance();
    //运行优化器
    void runOptimizeIR();
};



#endif //OPTIMIZEIR_H
