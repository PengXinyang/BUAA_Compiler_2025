//
// Created by PengXinyang on 24-11-23.
//

#include "../../../../include/value/architecture/data_structure/Loop.h"

int Loop::getLoopDepth() const {
    return loopDepth;
}

Loop::Loop(BasicBlock *Cond, BasicBlock *ForStmt2, BasicBlock *Stmt, BasicBlock *AfterBlock) {
    loopDepth = 0;
    this->Cond = Cond;
    this->ForStmt2 = ForStmt2;
    this->Stmt = Stmt;
    this->AfterBlock = AfterBlock;
}
