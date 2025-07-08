//
// Created by PengXinyang on 24-11-23.
//

#ifndef IOINSTRUCTION_H
#define IOINSTRUCTION_H
#include "value/architecture/user/Instruction.h"

//用于IO处理，其实就是给出的五个函数
/*
* declare i32 @getint()
* declare i32 @getchar()
* declare void @putint(i32)
* declare void @putch(i32)
* declare void @putstr(i8*)
*/


class IOInstruction :public Instruction{
public:
    IOInstruction() = default;
    IOInstruction(IRType* ir_type, const string& name, const string& instruction_type);
    static string getIODefine(){return "";}
};

#endif //IOINSTRUCTION_H
