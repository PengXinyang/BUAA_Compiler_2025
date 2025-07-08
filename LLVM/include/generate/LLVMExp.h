//
// Created by PengXinyang on 24-11-25.
//

#ifndef LLVMEXP_H
#define LLVMEXP_H
#include "../../../Parser/include/ParserTree.h"
#include "../../../Symbol/include/Symbol.h"
#include "value/Value.h"
#include "value/architecture/BasicBlock.h"


/**
* LLVM Exp类型的生成器，主要用来确定Exp相关情况，并且尽可能给const赋值
*/
class LLVMGenerate;
class LLVMExp {
private:
    LLVMGenerate* llvmGenerate{};
    shared_ptr<SymbolTable> now_symbol_table{};
    /**
     * genVarSymbolValueIr 用于数值对应的LLVM IR代码，
     * 提取了IntValue和ConstValue的公共部分
     * 形成了VarSymbol的一类处理方法，包括load和get指令
     */
    Value* generateVarValueIR(const vector<Value*>& values, Symbol* symbol);
public:
    LLVMExp() = default;
    explicit LLVMExp(LLVMGenerate* llvmGenerate);

    /**
     * 左值表达式 LVal → Ident ['[' Exp ']']
     * 获取左值对应的右侧LLVM IR代码，获得左值的地址
     */
    Value* generateAddressLLVM(TreeNode* AstRoot);
    /**
     * LVal -> Ident { '[' Exp ']'}
     * 用于创建左值对应的LLVM IR代码，获得左值的值
     */
    Value* generateLValIR(TreeNode* AstRoot);
    /**
    * Stmt -> LVal '=' 'getint' '(' ')'';'
    */
    Value* generateGetIntIR(const TreeNode* AstRoot);
    /**
    * Stmt -> LVal '=' 'getchar' '(' ')'';'
    */
    Value* generateGetCharIR(const TreeNode* AstRoot);
    /**
    * Cond -> LOrExp
    */
    void generateCondIR(TreeNode* AstRoot,BasicBlock* ifTrueBlock, BasicBlock* ifFalseBlock);
    /**
    * LorExp -> LAndExp | LOrExp '||' LAndExp
    */
    void generateLOrExpIR(TreeNode* AstRoot,BasicBlock* ifTrueBlock, BasicBlock* ifFalseBlock);
    /**
    * LAndExp -> EqExp | LAndExp '&&' EqExp
    */
    void generateLAndExpIR(TreeNode* AstRoot,BasicBlock* ifTrueBlock, BasicBlock* ifFalseBlock);
    /**
    * EqExp -> RelExp | EqExp ('=='|'!=') RelExp
    */
    void generateEqExpIR(TreeNode* AstRoot,BasicBlock* ifTrueBlock, BasicBlock* ifFalseBlock);
};



#endif //LLVMEXP_H
