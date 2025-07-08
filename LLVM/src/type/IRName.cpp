//
// Created by PengXinyang on 24-11-24.
//

#include "../../include/type/IRName.h"

#include "type/IRPreName.h"

unordered_map<Function*,int> IRName::blockMap;
unordered_map<Function*,int> IRName::localVariableMap;
unordered_map<Function*,int> IRName::paramMap;
int IRName::countStringId = 0;
Module* IRName::now_module = nullptr;
Function* IRName::now_function = nullptr;
BasicBlock* IRName::now_block = nullptr;
stack<Loop*> IRName::loopStack;

void IRName::setNowBlock(BasicBlock *block) {
    now_block = block;
}

BasicBlock *IRName::getNowBlock() {
    return now_block;
}

void IRName::setNowFunction(Function *function) {
    now_function = function;
}

Function *IRName::getNowFunction() {
    return now_function;
}

Loop *IRName::getNowLoop() {
    return loopStack.top();
}

void IRName::setNowModule(Module *module) {
    now_module = module;
}

string IRName::getBlockName(Function *function) {
    if(function == nullptr) function = now_function;
    int index;
    if(blockMap.find(function) == blockMap.end()) { index = 0;}
    else index = blockMap[function];
    blockMap[function] = index + 1;
    return function->value_name.substr(1) + "_" + IRPreName::toLLVM(IRPreType::BLOCK) + to_string(index);
}

string IRName::getLocalVariableName(Function *function) {
    if(function == nullptr) function = now_function;
    int index;
    if(localVariableMap.find(function) == localVariableMap.end()) { index = 0;}
    else index = localVariableMap[function];
    localVariableMap[function] = index + 1;
    return IRPreName::toLLVM(IRPreType::LOCAL_VAR) + to_string(index);
}

string IRName::getGlobalVariableName(const string& global_var_name) {
    return IRPreName::toLLVM(IRPreType::GLOBAL_VAR) + global_var_name;
}

string IRName::getFunctionName(const string& function_name) {
    return IRPreName::toLLVM(IRPreType::FUNCTION) + function_name;
}

string IRName::getConstStringName() {
    return IRPreName::toLLVM(IRPreType::CONST_STRING) + to_string(countStringId++);
}

string IRName::getParamName(Function *function) {
    if(function == nullptr) function = now_function;
    int index;
    if(paramMap.find(function) == paramMap.end()) { index = 0;}
    else index = paramMap[function];
    paramMap[function] = index + 1;
    return IRPreName::toLLVM(IRPreType::Param) + to_string(index);
}

void IRName::addFunction(Function *function) {
    now_module->addGlobalValue(function);
}

void IRName::addGlobalVariable(GlobalVariable *global_variable) {
    now_module->addGlobalValue(global_variable);
}

void IRName::addBasicBlock(BasicBlock *block) {
    now_function->addBasicBlock(block);
}

void IRName::addParam(Param *param) {
    now_function->addParam(param);
}

void IRName::addInstruction(Instruction *instruction) {
    now_block->addInstruction(instruction);
}

void IRName::addConstString(ConstString *const_string) {
    now_module->addConstantString(const_string);
}

void IRName::pushLoopStack(Loop *loop) {
    loopStack.push(loop);
}

void IRName::popLoopStack() {
    loopStack.pop();
}
