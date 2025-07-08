//
// Created by PengXinyang on 24-11-21.
//

#include "../../include/value/Value.h"

#include <algorithm>

#include "value/user/User.h"

Value::Value(IRType* type, const string &name, const vector<Use*> &useChain) {
    value_type = type;
    value_name = name;
    this->useChain = useChain;
}

Value::Value(IRType* type, const string &name) {
    value_type = type;
    value_name = name;
    this->useChain = vector<Use*>();
}

void Value::addUser(User* user) {
    useChain.push_back(new Use(user,this));
}

User* Value::removeUser(User* user) {
    const auto it = remove_if(useChain.begin(), useChain.end(),
                              [&user](const Use* use) {
                                  return use->getUser() == user;
                              });
    if(it!=useChain.end()) {
        useChain.erase(it);
        return user;
    }
    return nullptr;
}

void Value::replaceUser(Value *oldValue, Value *newValue, User *user) {
    if(oldValue!=nullptr && newValue!=nullptr) {
        oldValue->removeUser(user);
        newValue->addUser(user);
    }
}

vector<User *> Value::getAllUser() const {
    vector<User *> users;
    users.reserve(useChain.size());
    for(const auto & it : useChain) {
        users.push_back(it->getUser());
    }
    return users;
}

void Value::replaceAllUser(Value *value) {
    //防止删除导致循环发生变化
    const vector<Use*> copy_use_chain = vector(useChain);
    for(const auto use:copy_use_chain) {
        User *user = use->getUser();
        user->replaceValue(this,value);
    }
}
