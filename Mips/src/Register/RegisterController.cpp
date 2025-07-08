//
// Created by PengXinyang on 24-12-5.
//

#include "Register/RegisterController.h"

unordered_map<Value*,Register*> RegisterController::valueRegisterMap;

void RegisterController::allocateRegister(Value *value, Register *reg) {
    valueRegisterMap[value] = reg;
}
Register *RegisterController::getRegister(Value *value) {
    if(valueRegisterMap.find(value) == valueRegisterMap.end()) {return nullptr;}
    return valueRegisterMap[value];
}
