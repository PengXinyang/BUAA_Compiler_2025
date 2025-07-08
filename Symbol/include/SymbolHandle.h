//
// Created by PengXinyang on 24-10-12.
//

#ifndef SYMBOLHANDLE_H
#define SYMBOLHANDLE_H
#include <stack>

#include "../../Parser/include/ParserTree.h"
#include "Symbol.h"
#include "../../Parser/include/Parser.h"
using namespace std;

//处理符号表的类，单例模式
class SymbolHandle {
public:
    static SymbolHandle* getSymbolHandleInstance();
    void handleSymbol();
    void printSymbol();//输出全部的符号表
    shared_ptr<SymbolTable> getSymbolTable();//获取全局符号表
private:
    static SymbolHandle* instance;
    TreeNode*paserTree = nullptr;//语法树根节点
    int symbol_table_id = 0;//全局符号表序号，每创建一个符号表就+1
    shared_ptr<SymbolTable> GlobalSymbolTable = nullptr;
    shared_ptr<SymbolTable> nowSymbolTable = nullptr;
    bool is_main = false;//当前是否在main函数，用于处理main函数没有返回值的问题
    bool is_main_return = false;// 当前在main函数，有没有返回值
    int is_in_for = 0;//0表示不在循环，其余表示在第几层循环
    string func_token;//标记当前在哪个函数域内
    SymbolHandle();
    /**
     * 创建一个符号表
     * @return
     */
    shared_ptr<SymbolTable> createSymbolTable();

    /**
     * 主编译单元，从这里进去获取整个符号表
     * @param root
     */
    void SymbolCompUnit(TreeNode*root);
    void SymbolDecl(TreeNode* root);//当遍历树的节点是Decl时进入
    void SymbolConstDecl(TreeNode* root);//当树节点是ConstDecl时进入
    /**
     * 当树节点是Btype时进入
     * @param root
     * @return 类型，0是int 1是char
     */
    int SymbolBtype(const TreeNode* root);

    /**
     * 当树节点是ConstDef时进入
     * @param root
     * @param is_const 是否是常量
     * @param btype 类型
     */
    void SymbolConstDef(TreeNode* root, bool is_const, int btype);

    /**
     * 处理ConstInitVal
     * token用于赋值时找到对应的符号
     * @param root
     * @param token
     */
    void SymbolConstInitVal(TreeNode* root, const string& token);
    void SymbolVarDecl(TreeNode* root); // 变量声明
    void SymbolVarDef(TreeNode* root, bool is_const, int btype); // 变量定义
    void SymbolInitVal(TreeNode* root, const string& token); // 变量初值
    void SymbolFuncDef(TreeNode* root); // 函数定义
    void SymbolMainFuncDef(TreeNode* root); // 主函数定义
    int SymbolFuncType(TreeNode* root); // 函数类型
    /**
     * 函数形参表
     * @param root
     * @param params_type 参数类型的vector，用于存储在函数符号中
     */
    void SymbolFuncFParams(TreeNode* root, vector<int>& params_type);

    /**
     * 函数形参
     * @param root
     * @return 形参的类型，0是变量，1是int数组，2是char数组
     */
    int SymbolFuncFParam(TreeNode* root);

    /**
     * 语句块处理，在这里负责根据“是否由函数产生”建立符号表
     * @param root
     * @param is_func
     */
    void SymbolBlock(TreeNode* root, bool is_func); // 语句块
    /**
     * Block已建立符号表，所以在这里不需要再考虑是不是函数产生了
     * @param root
     */
    void SymbolBlockItem(TreeNode* root); // 语句块项，如果不是func要创建符号表，否则符号表在函数部分创建完成，不需要再创建
    void SymbolStmt(TreeNode* root); // 语句
    void SymbolForStmt(TreeNode* root); // for语句
    /**
     * 表达式
     * @param root
     * @param token token不为空，那么就把token对应的符号的值改变；如果token为空则不处理
     */
    int SymbolExp(TreeNode* root, const string& token); //
    void SymbolCond(TreeNode* root); // 条件表达式
    /**
     * 左值表达式
     * @param root
     * @param type 这个左值表达式的Ident是不是常量，0 不是常量也有定义，1 是常量，2 未定义
     * @return 第一个是LVal表达式算出来的值，第二个是数组标号Exp;如果Exp=-1说明不是数组
     */
    pair<int,int> SymbolLVal(TreeNode* root, int* type);
    int SymbolPrimaryExp(TreeNode* root, const string& token); // 基本表达式
    int SymbolNumber(TreeNode* root); // 数值
    char SymbolCharacter(TreeNode* root); // 字符
    int SymbolUnaryExp(TreeNode* root, const string& token); // 一元表达式
    char SymbolUnaryOp(TreeNode* root); // 单目运算符
    vector<int> SymbolFuncRParams(TreeNode* root, const string& func_token); // 函数实参表
    int SymbolMulExp(TreeNode* root, const string& token); // 乘除模表达式
    int SymbolAddExp(TreeNode* root, const string& token); // 加减表达式
    int SymbolRelExp(TreeNode* root); // 关系表达式
    int SymbolEqExp(TreeNode* root); // 相等性表达式
    int SymbolLAndExp(TreeNode* root); // 逻辑与表达式
    int SymbolLOrExp(TreeNode* root); // 逻辑或表达式
    /**
     * 常量表达式
     * @param root
     * @param token
     * @return 返回表达式的值
     */
    int SymbolConstExp(TreeNode* root, const string& token);
};


#endif //SYMBOLHANDLE_H
