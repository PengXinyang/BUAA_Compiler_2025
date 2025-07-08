//
// Created by PengXinyang on 24-11-24.
//

#ifndef LLVMSYMBOL_H
#define LLVMSYMBOL_H
#include "../../../Parser/include/ParserTree.h"
#include "../../../Symbol/include/Symbol.h"
#include "generate/LLVMGenerate.h"


/*
* 这个类用于遍历已有的符号表，采用单例模式
* 目的：将当前符号表中的符号和Value对应起来
* 在遍历语法树的过程中，只有定义语句涉及到符号和Value的关联
*/
class SymbolValue {
public:
    static SymbolValue* getInstance();
    //Symbol* ConnectSymbolValue(TreeNode* root);
    vector<Value*> genConstValuesIR(TreeNode *root);
    vector<Value*> genValuesIR(TreeNode *root);
private:
    static SymbolValue* instance;
    LLVMGenerate* generate;
    shared_ptr<SymbolTable> now_symbol_table;
    SymbolValue();
    //常量定义处理
    /*
    * ConstDef → Ident [ '[' ConstExp ']' ] '=' ConstInitVal
    */
    //Symbol* ConnectConstDef(TreeNode* root);
    //变量定义处理
    /*
    * VarDef → Ident [ '[' ConstExp ']' ]
    * | Ident [ '[' ConstExp ']' ] '=' InitVal
    */
    //Symbol* ConnectVarDef(TreeNode* root);
    //函数定义处理
    /*
    * FuncDef → FuncType Ident '(' [FuncFParams] ')' Block
    */
    //Symbol* ConnectFuncDef(TreeNode* root);
    //主函数定义处理
    /*
    * MainFuncDef → 'int' 'main' '(' ')' Block
    */
    //Symbol* ConnectMainFuncDef(TreeNode* root);
    //函数参数处理
    /*
    * FuncFParam → BType Ident ['[' ']']
    */
    //Symbol* ConnectFuncFParam(TreeNode* root);
};


#endif //LLVMSYMBOL_H
