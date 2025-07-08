//
// Created by PengXinyang on 24-11-23.
//

#ifndef CONST_H
#define CONST_H
#include "value/Value.h"

//常量类型，注意，对于常量，不能调用函数，只能调用常数和其它常量
class ConstValue :public Value{
private:
    long long num = 0;//常量的值，无论是int还是char类型
    bool defined = true;//该常量是否被定义；如果没有定义则应当赋值成0
public:
    [[nodiscard]] long long getNum() const {
        return num;
    }
    [[nodiscard]] bool isDefined() const {
        return defined;
    }
    ConstValue() = default;
    ConstValue(IRType* ir_type,const string& value,bool defined = true);
};


#endif //CONST_H
