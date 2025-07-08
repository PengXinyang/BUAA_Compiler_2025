//
// Created by PengXinyang on 24-12-18.
//

#ifndef OPTIMIZEMIPS_H
#define OPTIMIZEMIPS_H
#include "value/architecture/Module.h"

/**
* mips代码优化类
*/
class OptimizeMips {
private:
    static OptimizeMips* instance;
    Module* module = nullptr;
    OptimizeMips();
public:
    static OptimizeMips* getInstance();
    void runOptimizeMips();
};



#endif //OPTIMIZEMIPS_H
