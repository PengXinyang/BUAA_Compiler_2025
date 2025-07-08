//
// Created by PengXinyang on 24-11-21.
//

#ifndef TYPE_H
#define TYPE_H
#include<string>
using namespace std;

class IRType {
public:
    virtual ~IRType() = default;
    virtual bool isVoid();
    virtual bool isChar();
    virtual bool isInt();
    virtual bool isBool();
    virtual bool isArray();
    virtual bool isPointer();//判断是不是指针类型
    virtual bool isBlock();//判断是不是块类型
    virtual string toLLVM();
};



#endif //TYPE_H
