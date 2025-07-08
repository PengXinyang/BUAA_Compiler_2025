//
// Created by PengXinyang on 24-11-21.
//

#ifndef IRPOINTER_H
#define IRPOINTER_H
#include "../IRType.h"

//生成指针类型
//注意：函数传参是数组时，其实传的是指针
//对于数组的赋值，可能涉及到二重指针的情况。对于二重指针，可以看作“指针的指针”，即IRType还是IRPointer的形式
class IRPointer :public IRType{
private:
    IRType* irType = nullptr;

public:
    IRPointer() =  default;
    explicit IRPointer(IRType* ir_type);
    [[nodiscard]] IRType* ir_type() const {
        return irType;
    }
    bool isPointer() override;
    string toLLVM() override;
};



#endif //IRPOINTER_H
