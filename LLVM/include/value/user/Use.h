//
// Created by PengXinyang on 24-11-21.
//

#ifndef USE_H
#define USE_H
#include "../Value.h"

class Value;
class User;
//记录User使用Value的数据结构
class Use {
private:
    User* user;
    Value* value;
public:
    Use(User* user, Value* value);
    Use() = default;
    [[nodiscard]] User* getUser() const {
        return user;
    }
    void setUser(User* user) {
        this->user = user;
    }
    [[nodiscard]] Value* getValue() const {
        return value;
    }
    void setValue(Value* value) {
        this->value = value;
    }
};



#endif //USE_H
