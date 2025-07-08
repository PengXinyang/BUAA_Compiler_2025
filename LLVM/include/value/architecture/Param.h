//
// Created by PengXinyang on 24-11-23.
//

#ifndef PARAM_H
#define PARAM_H
#include "user/Function.h"
#include "value/Value.h"


class Param :public Value{
private:
    Function*func = nullptr;//参数所属的函数
public:
    [[nodiscard]] Function * getFunc() const {
        return func;
    }

    void setFunc(Function *func) {
        this->func = func;
    }
    Param() = default;
    Param(IRType* ir_type, const string& name);
    string toLLVM() override;
};



#endif //PARAM_H
