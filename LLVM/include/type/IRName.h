//
// Created by PengXinyang on 24-11-24.
//

#ifndef IRNAME_H
#define IRNAME_H
#include <stack>
#include <unordered_map>

#include "value/architecture/Module.h"
#include "value/architecture/user/Function.h"

/*
* 用于处理命名
* 处理规则：
* 对于全局符号，统一为“@”加符号名
* 对于局部符号，这里采用一个全局的计数器，每次生成名字后就+1
* 块号同理
* 对于字符串常量，统一用@.str.数字进行命名
*/
class IRName {
private:
    //记录块所在的函数和块号id的关系
    static unordered_map<Function*,int> blockMap;
    //记录块里的局部变量所在的函数和变量id的关系
    //不同块的变量号是不同的
    static unordered_map<Function*,int> localVariableMap;
    //形参Map
    static unordered_map<Function*,int> paramMap;
    //记录常量字符串的id
    static int countStringId;
    //记录当前的模块
    static Module *now_module;
    //记录当前所在的基本块
    static BasicBlock *now_block;
    //记录当前所在的函数
    static Function *now_function;
    //采用一个Stack来存储，每次进入循环时push，退出循环时pop
    static stack<Loop*> loopStack;

public:
    //获取基本块的名字
    static string getBlockName(Function *function = now_function);
    //获取局部变量名
    static string getLocalVariableName(Function *function = now_function);
    //获取全局变量名
    static string getGlobalVariableName(const string& global_var_name);
    //获取函数名
    static string getFunctionName(const string& function_name);
    //获取字符串常量名
    static string getConstStringName();
    //获取形参名
    static string getParamName(Function *function = now_function);
    //获取当前所在的循环
    static Loop* getNowLoop();
    static void setNowModule(Module *module);
    static void setNowBlock(BasicBlock *block);
    static void setNowFunction(Function *function);
    static BasicBlock *getNowBlock();
    static Function *getNowFunction();
    static void addFunction(Function *function);
    static void addGlobalVariable(GlobalVariable* global_variable);
    static void addBasicBlock(BasicBlock* block);
    static void addParam(Param* param);
    static void addInstruction(Instruction* instruction);
    static void addConstString(ConstString* const_string);
    //压循环栈
    static void pushLoopStack(Loop* loop);
    //弹循环栈
    static void popLoopStack();
};



#endif //IRNAME_H
