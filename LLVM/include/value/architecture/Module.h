//
// Created by PengXinyang on 24-11-23.
//

#ifndef MODULE_H
#define MODULE_H
#include "ConstString.h"
#include "user/Function.h"
#include "user/GlobalValue.h"
#include "user/GlobalVariable.h"
#include "value/Value.h"


class Module :public Value{
private:
    //在模块module中，包含全局变量和函数
    vector<GlobalVariable*> globalVariables;
    vector<Function*> functions;
    //对于printf的格式字符串，需要单独存
    vector<ConstString*> constantStrings;
public:
    Module();
    //添加一个全局符号
    void addGlobalValue(GlobalValue* globalValue);
    //从全局符号获取所有的全局变量
    vector<GlobalVariable*>& getGlobalValues();
    //从全局符号获取所有的函数
    vector<Function*>& getFunctions();
    //添加格式字符串
    void addConstantString(ConstString* constantString);
    string toLLVM() override;
    void generateMIPS() override;

    //-------------------------以下用于代码优化----------------------//
    //删除每个函数基本块内无用的代码和块
    void DCEBlock();
    //构建控制流图
    void buildCfgGraph();
    //Debug用，输出控制流图
    void printCFG();
    //生成支配树
    void generateDominantTree();
    //Debug用，输出支配树
    void printDominantTree();
    //进行活跃变量分析
    void activeAnalysis();
    //Debug用，输出活跃变量
    void printActiveAnalysis();
    //debug用，输出寄存器的分配
    void printRegisterDistribute();
};



#endif //MODULE_H
