//
// Created by PengXinyang on 24-11-23.
//

#include "../../../include/value/architecture/Param.h"

#include "optimize/LLVMOptimizerInit.h"
#include "type/IRName.h"

Param::Param(IRType* ir_type, const string& name) :Value(ir_type,name){
    //优化部分
    if(!LLVMOptimizerInit::isOptimize()) {
        IRName::addParam(this);
    }
}

string Param::toLLVM() {
    return value_type->toLLVM() + " " + value_name;
}
