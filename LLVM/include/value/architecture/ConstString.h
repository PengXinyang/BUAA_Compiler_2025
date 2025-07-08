//
// Created by PengXinyang on 24-11-23.
//

#ifndef CONSTSTRING_H
#define CONSTSTRING_H
#include "type/irType/IRPointer.h"
#include "value/Value.h"

//存储常量字符串，用于printf和字符数组赋值
class ConstString :public Value{
private:
    string str;//常量字符串内容
public:
    ConstString() = default;
    ConstString(const string &name, const string &str, const vector<int> &dims);
    [[nodiscard]] IRPointer *getIRPointer() const;
    [[nodiscard]] string getString() const;
    string toLLVM() override;
    void generateMIPS() override;
};



#endif //CONSTSTRING_H
