//
// Created by PengXinyang on 24-11-22.
//

#ifndef INITVAR_H
#define INITVAR_H
#include <vector>

#include "../../../type/IRType.h"


class InitVar {
private:
    //全局变量的类型，关于size和维度的信息在IRArray中有记录
    //如果初始值是数组，必须记录数组
    IRType* type{};
    vector<int> initValues;//初始值大小
    string initString;//对于char数组，可能直接赋值字符串
public:
    [[nodiscard]] IRType* getType() const {
        return type;
    }

    [[nodiscard]] vector<int>& getInitValues(){
        return initValues;
    }
    [[nodiscard]] string getInitString() const {
        return initString;
    }
    InitVar() = default;
    InitVar(IRType* type, const vector<int>& initValues, const string& initString);
    string toLLVM();
};



#endif //INITVAR_H
