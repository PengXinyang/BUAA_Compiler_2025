//
// Created by PengXinyang on 24-11-23.
//

#ifndef IOGETINT_H
#define IOGETINT_H
#include "IOInstruction.h"


class IOGetInt :public IOInstruction{
public:
    IOGetInt();
    IOGetInt(const string& name,const string& instruction_type);
    //获取getint的定义语句
    static string getIODefine();
    string toLLVM() override;
    void generateMIPS() override;
};



#endif //IOGETINT_H
