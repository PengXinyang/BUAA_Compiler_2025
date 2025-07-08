//
// Created by PengXinyang on 24-11-21.
//

#include "../../../include/value/user/User.h"

#include <algorithm>

User::User(IRType* type, const string &user_name) :Value(type,user_name){
    opValueChain = vector<Value*>();
}

void User::addOpValue(Value *op_value, const int cnt) {
    for (int i = 0; i < cnt; i++) {
        opValueChain.push_back(op_value);
        if(op_value) {
            op_value->addUser(this);
        }
    }
}

void User::replaceValue(Value *old_value, Value *new_value) {
    if(!old_value) {//如果old_value本身不存在，直接退出
        return;
    }
    for(auto it = opValueChain.begin(); it != opValueChain.end(); it++) {
        if(*it == old_value) {
            if(new_value) {
                *it = new_value;
            }else {
                opValueChain.erase(it);
            }
            break;
        }
    }
    replaceUser(old_value, new_value,this);
}

void User::dropOpValue() {
    for(Value* op_value : opValueChain) {
        vector<Use*>& op_use_chain = op_value->getUseChain();
        auto it = remove_if(op_use_chain.begin(), op_use_chain.end(),
            [this](const Use* use) {
                return use->getUser() == this;
        });
        if (it != op_use_chain.end()) {
            op_use_chain.erase(it, op_use_chain.end()); // 删除指定范围的元素
        }
    }
}
