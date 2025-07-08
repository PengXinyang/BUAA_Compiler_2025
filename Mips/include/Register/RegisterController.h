//
// Created by PengXinyang on 24-12-5.
//

#ifndef REGISTERCONTROLLER_H
#define REGISTERCONTROLLER_H
#include <unordered_map>

#include "structure/Register.h"
#include "value/Value.h"
using namespace std;
/**
* RegisterController 是一个寄存器控制器
* 主要用于寄存器的分配
*/
class RegisterController {
private:
    static unordered_map<Value*,Register*> valueRegisterMap;

public:
    [[nodiscard]] static unordered_map<Value *, Register *> getValueRegisterMap() {
        return valueRegisterMap;
    }

    static void setValueRegisterMap(const unordered_map<Value *, Register *> &value_register_map) {
        valueRegisterMap = value_register_map;
    }
    static void allocateRegister(Value* value, Register *reg);//分配寄存器
    static Register *getRegister(Value *value);
};



#endif //REGISTERCONTROLLER_H
