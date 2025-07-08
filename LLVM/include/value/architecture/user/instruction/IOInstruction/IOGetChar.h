//
// Created by PengXinyang on 24-11-23.
//

#ifndef IOGETCHAR_H
#define IOGETCHAR_H
#include "IOInstruction.h"


class IOGetChar :public IOInstruction{
public:
    IOGetChar();
    IOGetChar(const string& name,const string& instruction_type);
    //获取getchar定义语句
    static string getIODefine();
    string toLLVM() override;
    void generateMIPS() override;
};



#endif //IOGETCHAR_H
