//
// Created by PengXinyang on 24-11-21.
//

#ifndef USER_H
#define USER_H
#include "../Value.h"

class Value;
class Use;
//根据教程“LLVM的数据结构"，可知User类继承Value类并且可以使用Value
//User使用Value的关系由Use记录
class User :public Value{
public:
    vector<Value*> opValueChain;//这个User使用的操作数
    [[nodiscard]] vector<Value *>& op_value_chain(){
        return opValueChain;
    }
    User() = default;
    User(IRType* type, const string &user_name);
    void addOpValue(Value* op_value, int cnt = 1);//添加一个操作数Value
    void replaceValue(Value* old_value,Value* new_value);//替换一个操作数
    void dropOpValue();//删除所有该User中的该操作数
};



#endif //USER_H
