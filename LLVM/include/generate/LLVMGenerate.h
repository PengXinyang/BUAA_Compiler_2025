//
// Created by PengXinyang on 24-11-25.
//

#ifndef LLVMGENERATE_H
#define LLVMGENERATE_H
#include "../../../Symbol/include/SymbolHandle.h"
#include "LLVMSymbol/SymbolCalculate.h"


/*
* LLVM语言生成器，单例模式
* 采用递归下降法语法制导翻译
*/
class LLVMExp;
class SymbolValue;
class GenerateIR;
class LLVMGenerate {
public:
    shared_ptr<SymbolTable> now_symbol_table{};//当前所在的符号表
    LLVMGenerate();
    /*
     * 全局LLVM Value的生成器
     */
    Value* generateLLVMIR(TreeNode* AstRoot);
    [[nodiscard]] shared_ptr<SymbolTable> getNowSymbolTable() const {
        return now_symbol_table;
    }
    static LLVMGenerate* getInstance();
private:
    static LLVMGenerate* instance;
    GenerateIR* generateIR{};
    SymbolCalculate* symbol_calculate_;
    SymbolValue* symbol_value_instance{};
    //获取符号表实例
    shared_ptr<SymbolTable> symbol_table{};
    LLVMExp* llvmExp{};
    Value* CompUnitIR(TreeNode* AstRoot);
    //定义式
    Value* ConstDefIR(TreeNode* AstRoot);
    Value* VarDefIR(TreeNode* AstRoot);
    Value* BlockIR(TreeNode* AstRoot);
    Value* IfIR(TreeNode* AstRoot);
    Value* ForIR(TreeNode* AstRoot);
    Value* ContinueIR(TreeNode* AstRoot);
    Value* BreakIR(TreeNode* AstRoot);
    /**
    * Stmt -> 'return' [Exp] ';'
    */
    Value* ReturnIR(TreeNode* AstRoot);
    Value* PrintfIR(TreeNode* AstRoot);
    Value* ExpIR(TreeNode* AstRoot);//用于Exp和ConstExp，只有一个子节点
    Value* PrimaryExpIR(TreeNode* AstRoot);
    Value* NumberOrCharIR(TreeNode* AstRoot);//用于将数字和字母存成常量
    Value* UnaryExpIR(TreeNode* AstRoot);
    Value* MulExpIR(TreeNode* AstRoot);
    Value* AddExpIR(TreeNode* AstRoot);
    Value* RelExpIR(TreeNode* AstRoot);
    Value* EqExpIR(TreeNode* AstRoot);
    //函数式
    Value* FuncDefIR(TreeNode* AstRoot);//函数定义
    Value* FuncFParamIR(TreeNode* AstRoot);//函数形参
    Value* MainFuncDefIR(TreeNode* AstRoot);//主函数定义
    //等于号赋值
    Value* AssignIR(TreeNode* AstRoot);

    //进入新的符号表
    void enterNewSymbolTable();
    //退出符号表
    void quitNowSymbolTable();
    //强制类型转换，统一1和2的Value类型，只用在了AddExp和MulExp
    void changeType(Value** value1,Value** value2);
};



#endif //LLVMGENERATE_H
