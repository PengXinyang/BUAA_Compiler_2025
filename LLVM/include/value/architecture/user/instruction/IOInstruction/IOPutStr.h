//
// Created by PengXinyang on 24-11-23.
//

#ifndef IOPUTSTR_H
#define IOPUTSTR_H
#include "IOInstruction.h"
#include "value/architecture/ConstString.h"


class IOPutStr : public IOInstruction{
private:
    ConstString* constString;//输出的字符串
public:
    explicit IOPutStr(ConstString* constString);
    static string getIODefine();
    string toLLVM() override;
    void generateMIPS() override;
};



#endif //IOPUTSTR_H
