//
// Created by PengXinyang on 24-11-25.
//

#ifndef SYMBOLCALCULATE_H
#define SYMBOLCALCULATE_H
#include <vector>

#include "../../../Parser/include/ParserTree.h"
using namespace std;

/**
* SymCalc 是语义分析用于计算表达式的工具类,主要可以将位于全局区
* 的全局变量的值在编译时期进行计算,并将其值存入符号表中，同时也
* 担任了部分常数表达式的化简工作
*/
class SymbolCalculate {
private:
    SymbolCalculate() = default;
    static SymbolCalculate * instance;
public:
    static SymbolCalculate * getInstance();
    /**
    * 计算表达式的主函数,根据类型采用不同类型分别的处理函数
    */
    int calculate(TreeNode* root);
    /**
    * 用于存储分析得出的ValDef的初始化值。由于需要处理变量和数组，所以返回一个数组
    */
    vector<int> calculateInitVal(TreeNode *root);
    /**
    * 用于存储分析得出的ConstDef的初始化值。由于需要处理变量和数组，所以返回一个数组
    */
    vector<int> calculateConstInitVal(TreeNode *root);
private:
    int calculateAddExp(TreeNode* root);
    int calculateMulExp(TreeNode* root);
    int calculateUnaryExp(TreeNode* root);
    int calculatePrimaryExp(TreeNode* root);
    int calculateNumberOrChar(TreeNode* root);
    int calculateLVal(TreeNode* root);
};



#endif //SYMBOLCALCULATE_H
