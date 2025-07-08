//
// Created by PengXinyang on 24-11-21.
//

#ifndef VALUE_H
#define VALUE_H
#include <vector>
#include <sstream>
#include "../type/IRType.h"
#include "./user/Use.h"
#include "instanceof.h"
#include <algorithm>

//防止循环依赖
class Use;
class User;
//在LLVM中，一切皆Value，这个类是所有类的基类
class Value {
public:
    virtual ~Value() = default;

    IRType* value_type = nullptr;//value的类型
    string value_name;//value的名字
    vector<Use*> useChain;//Value的定义使用链
    Value() = default;
    Value(IRType* type, const string &name);
    Value(IRType* type, const string &name, const vector<Use*> &useChain);

    /**
     * 下面几个函数是get和set
     */
    [[nodiscard]] IRType* getValueType() const{
        return value_type;
    }
    void setValueType(IRType* value_type) {
        this->value_type = value_type;
    }
    [[nodiscard]] string getValueName() const {
        return value_name;
    }
    void setValueName(const string &value_name) {
        this->value_name = value_name;
    }
    [[nodiscard]] vector<Use*>& getUseChain(){
        return useChain;
    }
    void setUseChain(const vector<Use*> &use_chain) {
        useChain = use_chain;
    }
    //添加一个使用者，User使用了这个Value
    void addUser(User* user);
    //移除一个使用者
    User* removeUser(User* user);
    //使用者原本使用的Value替换为这个Value
    void replaceUser(Value* oldValue,Value* newValue,User* user);
    //获取使用这个Value的全部User
    [[nodiscard]] vector<User*> getAllUser() const;
    //将所有的使用本Value的使用者全部替换为使用新的Value
    /**
    * 注意：存在严重问题：如果直接访问opValueChain, 那么在移出元素后会让迭代器指的位置出现问题
    * 所以遍历应当单独生成一个副本，防止迭代器出现偏移
    * @param value
    */
    void replaceAllUser(Value* value);
    //生成汇编的函数，具体到不同的类再具体重写
    void virtual generateMIPS(){}
    //保留toLLVM的方法，生成中间代码字符串
    string virtual toLLVM() { return ""; }

    //---------------以下是代码优化部分--------------//
    //插入phi指令
    virtual void insertPhiInstruction(){}
};



#endif //VALUE_H
