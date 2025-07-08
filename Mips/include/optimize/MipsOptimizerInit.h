//
// Created by PengXinyang on 24-12-10.
//

#ifndef MIPSOPTIMIZEINIT_H
#define MIPSOPTIMIZEINIT_H
#include "value/architecture/Module.h"


class MipsOptimizerInit {
private:
    static bool is_optimize;//表示是否优化
public:
    MipsOptimizerInit() = default;
    ~MipsOptimizerInit() = default;
    static bool isOptimize() {
        return is_optimize;
    }
    static void setIsOptimize(bool optimize) {
        is_optimize = optimize;
    }
    static void init(Module* module);
};



#endif //MIPSOPTIMIZEINIT_H
