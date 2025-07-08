//
// Created by PengXinyang on 24-11-25.
//

#include "../include/GenerateIR.h"

#include <fstream>
#include <iostream>
#include<regex>

#include "../../Config/include/ConfigOptimizer.h"
#include "../../Parser/include/Parser.h"
#include "generate/LLVMGenerate.h"
#include "optimize/LLVMOptimizerInit.h"
#include "type/IRName.h"

LLVMGenerate* GenerateIR::generate = nullptr;
Module* GenerateIR::module = nullptr;
GenerateIR* GenerateIR::instance = nullptr;
GenerateIR::GenerateIR() {
    root = Parser::getParserInstance()->getPaserTree();
    module = new Module();
    generate = LLVMGenerate::getInstance();
    IRName::setNowModule(module);
}
GenerateIR *GenerateIR::getInstance() {
    if (instance == nullptr) {
        instance = new GenerateIR();
    }
    return instance;
}
Module *GenerateIR::getModule() {
    return module;
}

void GenerateIR::printLLVMIR() {
    //实际上就是输出module的toLLVM
    string LLVM = module->toLLVM();
    ofstream outFile("llvm_ir.txt");
    // 检查文件是否成功打开
    if (!outFile) {
        cerr << "Error: Unable to open file!" << endl;
    }
    // 将字符串写入文件
    outFile << LLVM;
    // 关闭文件
    outFile.close();
}

void GenerateIR::generateLLVMIR() {
    LLVMOptimizerInit::setIsOptimize(false);
    generate->generateLLVMIR(root);
    LLVMOptimizerInit::setIsOptimize(true);
}

void GenerateIR::traverseTreeIR(TreeNode *root) {
    if(root->son_num==0) {
        //说明是叶子节点
        return;
    }
    const regex pattern("IFTK|FORTK|BREAKTK|CONTINUETK|RETURNTK|PRINTFTK|ASSIGN");
    for(const auto son:root->sonNode) {
        generate->generateLLVMIR(son);
        //如果是return，分析后可以直接退出，否则在这里继续分析Exp会造成重复
        if(regex_match(son->word.word_type, pattern)) return;
    }
}
