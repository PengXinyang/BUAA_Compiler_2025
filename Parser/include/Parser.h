//
// Created by PengXinyang on 24-9-28.
//

#ifndef PARSER_H
#define PARSER_H
#include "ParserTree.h"
#include "TokenScanner.h"

class Parser {
public:
    void ParserHandle();
    //输出至正确的文本
    void printParser();
    static Parser* getParserInstance();
    TreeNode* getPaserTree();
private:
    TreeNode *ParserRoot = nullptr;
    static Parser *instance;
    TokenScanner*tokenScanner = TokenScanner::getTokenScannerInstance();
    Word now_word;
    Word*preWords = nullptr;
    TreeNode* CompUnit();//编译单元
    TreeNode* Decl();//声明
    TreeNode* ConstDecl();//常量声明
    TreeNode* Btype();//基本类型
    TreeNode* ConstDef();//常量定义
    TreeNode* ConstInitVal();//常量初值
    TreeNode* VarDecl();//变量声明
    TreeNode* VarDef();//变量定义
    TreeNode* InitVal();//变量初值
    TreeNode* FuncDef();//函数定义
    TreeNode* MainFuncDef();//主函数定义
    TreeNode* FuncType();//函数类型
    TreeNode* FuncFParams();//函数形参表
    TreeNode* FuncFParam();//函数形参
    TreeNode* Block();//语句块
    TreeNode* BlockItem();//语句块项
    TreeNode* Stmt();//语句
    TreeNode* ForStmt();//for语句
    TreeNode* Exp();//表达式
    TreeNode* Cond();//条件表达式
    TreeNode* LVal();//左值表达式
    TreeNode* PrimaryExp();//基本表达式
    TreeNode* Number();//数值
    TreeNode* Character();//字符
    TreeNode* UnaryExp();//一元表达式
    TreeNode* UnaryOp();//单目运算符
    TreeNode* FuncRParams();//函数实参表
    TreeNode* MulExp();//乘除模表达式
    TreeNode* AddExp();//加减表达式
    TreeNode* RelExp();//关系表达式
    TreeNode* EqExp();//相等性表达式
    TreeNode* LAndExp();//逻辑与表达式
    TreeNode* LOrExp();//逻辑或表达式
    TreeNode* ConstExp();//常量表达式
};

class CompUnitNode final : public treeNode{
public:
    treeNode* parser() override;
    void symbol() override;
};

#endif //PARSER_H