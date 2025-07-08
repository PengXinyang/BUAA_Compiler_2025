//
// Created by PengXinyang on 24-11-22.
//

#ifndef GLOBALVARIABLE_H
#define GLOBALVARIABLE_H
#include "GlobalValue.h"
#include "value/architecture/data_structure/InitVar.h"


//全局变量类，继承自User
class GlobalVariable :public GlobalValue{
private:
    //InitVar作为初始化全局变量
    InitVar* initVar{};
    bool is_const_array = false;//是不是常数数组，对于常量，可以直接合并，不保留
public:
    [[nodiscard]] InitVar* getInitVar() const {
        return initVar;
    }
    GlobalVariable() = default;
    GlobalVariable(IRType* ir_type,const string& name, InitVar* init_var, bool is_const_array = false);
    void SetInitVar(InitVar* init_var) {
        initVar = init_var;
    }
    string toLLVM() override;
    //生成mips用
    void generateMIPS() override;
};



#endif //GLOBALVARIABLE_H
