//
// Created by PengXinyang on 24-11-23.
//

#ifndef GLOBALVALUE_H
#define GLOBALVALUE_H
#include "value/user/User.h"

//相当于全局符号类，向下有全局变量和函数，只起统一的作用
class GlobalValue :public User{
public:
    GlobalValue() = default;
    GlobalValue(IRType* ir_type, const string& name);
};



#endif //GLOBALVALUE_H
