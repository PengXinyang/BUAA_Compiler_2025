//
// Created by PengXinyang on 24-11-23.
//

#include "../../../include/value/architecture/Module.h"

#include <fstream>
#include <iostream>
#include <sstream>

#include "instanceof.h"
#include "optimize/Tool/ActiveVarAnalysis.h"
#include "optimize/Tool/DCE.h"
#include "structure/text/MipsBlock.h"
#include "structure/text/MipsInstruction/JalInstruction.h"
#include "structure/text/MipsInstruction/JInstruction.h"
#include "structure/text/MipsInstruction/SyscallInstruction.h"
#include "structure/text/PseudoInstruction/LiInstruction.h"
#include "type/irType/IRBlock.h"
#include "value/architecture/user/instruction/IOInstruction/IOGetChar.h"
#include "value/architecture/user/instruction/IOInstruction/IOGetInt.h"
#include "value/architecture/user/instruction/IOInstruction/IOPutCh.h"
#include "value/architecture/user/instruction/IOInstruction/IOPutInt.h"
#include "value/architecture/user/instruction/IOInstruction/IOPutStr.h"

Module::Module() :Value(new IRBlock("module"),"main_module"){
    functions = vector<Function*>();
    globalVariables = vector<GlobalVariable*>();
    constantStrings = vector<ConstString*>();
}
void Module::addGlobalValue(GlobalValue *globalValue) {
    if(instanceof<Function>(globalValue)) {
        auto* f = dynamic_cast<Function*>(globalValue);
        functions.push_back(f);
    }
    else if(instanceof<GlobalVariable>(globalValue)) {
        auto* g = dynamic_cast<GlobalVariable*>(globalValue);
        globalVariables.push_back(g);
    }
}

vector<GlobalVariable *>& Module::getGlobalValues() {
    return globalVariables;
}

vector<Function *>& Module::getFunctions() {
    return functions;
}

void Module::addConstantString(ConstString *constantString) {
    constantStrings.push_back(constantString);
}

string Module::toLLVM() {
    ostringstream os;
    //首先，添加输入输出函数的定义语句
    os<<IOGetInt::getIODefine()<<"\n";
    os<<IOGetChar::getIODefine()<<"\n";
    os<<IOPutInt::getIODefine()<<"\n";
    os<<IOPutCh::getIODefine()<<"\n";
    os<<IOPutStr::getIODefine()<<"\n\n";
    //其次，添加format语句
    for(const auto constantString : constantStrings) {
        os << constantString->toLLVM() << "\n";
    }
    os<<"\n";
    //最后，添加全局符号语句，包括全局变量和函数，理论上是按照出现的先后顺序
    for(const auto global_value : globalVariables) {
        os << global_value->toLLVM() << "\n";
    }
    for(const auto functon : functions) {
        os << functon->toLLVM() << "\n";
    }
    return os.str();
}

void Module::generateMIPS() {
    vector<GlobalVariable *> global_variables = getGlobalValues();
    vector<Function *> functions = getFunctions();
    for(const auto global_variable : global_variables) {
        global_variable->generateMIPS();
    }
    for(const auto constantString : constantStrings) {
        constantString->generateMIPS();
    }
    new JalInstruction("main");
    new JInstruction("end");
    for(const auto function : functions) {
        function->generateMIPS();
    }
    new MipsBlock("end");
    new LiInstruction(Register::getRegister(RegisterName::$v0),10);
    new SyscallInstruction();
}
void Module::DCEBlock() {
    //首先删除无用的指令
    for(const auto function:functions) {
        function->DCEBlockInstruction();
    }
    //然后删除无用的基本块
    for(const auto function:functions) {
        DCE::deleteBlock(function);
    }
}

void Module::buildCfgGraph() {
    for(const auto function : functions) {
        function->buildCfgGraph();
    }
}

void Module::printCFG() {
    const string filename = "cfg_debug.txt";
    ofstream file(filename,ios::out);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return;
    }
    file<<"控制流图如下"<<"\n\t";
    for(const auto function : functions) {
        file<<function->getCfgGraph()->printCFG()<<"\n\t";
    }
}

void Module::generateDominantTree() {
    for(const auto function : functions) {
        function->buildDominantTree();
    }
}

void Module::printDominantTree() {
    const string filename = "dominant_tree_debug.txt";
    ofstream file(filename,ios::out);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return;
    }
    file<<"支配树如下"<<"\n\t";
    for(const auto function : functions) {
        file<<"函数: "<<function->value_name<<"\n\t\t";
        file<<function->getDominantTree()->printDominantTree()<<"\n\t";
    }
}

void Module::activeAnalysis() {
    for(const auto function : functions) {
        function->activeAnalysis();
    }
}

void Module::printActiveAnalysis() {
    const string filename = "active_analysis_debug.txt";
    ofstream file(filename,ios::out);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return;
    }
    file<<"活跃变量分析如下"<<"\n\t";
    for(const auto function : functions) {
        file<<"函数: "<<function->value_name<<"\n\t\t";
        //file<<function->getActiveVarAnalysis()->printActiveAnalysis()<<"\n\t";
    }
}

void Module::printRegisterDistribute() {
    const string filename = "register_distribute_debug.txt";
    ofstream file(filename,ios::out);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return;
    }
    file<<"寄存器分配规则如下"<<"\n\t";
    for(const auto function : functions) {
        file<<"函数: "<<function->value_name<<"\n\t\t";
        file<<function->printRegisterDistribute()<<"\n\t";
    }
}
