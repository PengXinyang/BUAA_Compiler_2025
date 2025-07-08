//
// Created by PengXinyang on 24-11-23.
//

#include "../../../include/value/architecture/ConstValue.h"

ConstValue::ConstValue(IRType* ir_type, const string& num, const bool defined) :Value(ir_type,num){
    this->defined = defined;
    this->num = stoll(num);
}
