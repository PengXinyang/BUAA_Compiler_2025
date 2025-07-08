//
// Created by PengXinyang on 24-11-23.
//

#ifndef LOOP_H
#define LOOP_H
#include "value/architecture/BasicBlock.h"


//处理循环用，没有继承
/**
* 'for' '(' [ForStmt] ';' [Cond] ';' [ForStmt] ')' Stmt
* 其中，ForStmt和Cond均可以缺省
* 经过尝试，发现只有Cond，Stmt和ForStmt2会生成新的基本块
* 并且事实上Stmt和ForStmt2的基本块可以合并
*/
class Loop {
private:
    int loopDepth = 0;//循环深度
    BasicBlock* Cond{};//循环中的条件块
    BasicBlock* ForStmt2{};//第二个基本块
    BasicBlock* Stmt{};//跟在for循环后面的Stmt基本块
    BasicBlock* AfterBlock{};//跳出循环后的基本块
public:
    Loop() = default;
    Loop(BasicBlock* Cond, BasicBlock* ForStmt2, BasicBlock* Stmt,BasicBlock* AfterBlock);
    [[nodiscard]] int getLoopDepth() const;//获取当前循环深度，第几层循环
    [[nodiscard]] BasicBlock * getCondBlock() const {
        return Cond;
    }
    [[nodiscard]] BasicBlock * getForStmt2Block() const {
        return ForStmt2;
    }
    [[nodiscard]] BasicBlock * getStmtBlock() const {
        return Stmt;
    }
    [[nodiscard]] BasicBlock * getAfterBlock() const {
        return AfterBlock;
    }
};



#endif //LOOP_H
