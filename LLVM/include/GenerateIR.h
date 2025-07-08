//
// Created by PengXinyang on 24-11-25.
//

#ifndef GENERATEIR_H
#define GENERATEIR_H
#include "../../Parser/include/ParserTree.h"
#include "value/architecture/Module.h"


/*
* 生成LLVM IR的主类
*/
class LLVMGenerate;
class GenerateIR {
private:
    TreeNode* root{};//语法树根节点
    static LLVMGenerate* generate;
    static Module* module;
    static GenerateIR* instance;
    GenerateIR();
public:
    static GenerateIR* getInstance();
    Module* getModule();
    //生成LLVM IR的过程
    void generateLLVMIR();
    //输出LLVM代码
    void printLLVMIR();
    //遍历语法树的过程
    void traverseTreeIR(TreeNode* root);
};



#endif //GENERATEIR_H
