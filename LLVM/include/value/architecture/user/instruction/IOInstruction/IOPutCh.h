//
// Created by PengXinyang on 24-11-23.
//

#ifndef IOPUTCH_H
#define IOPUTCH_H
#include "IOInstruction.h"


class IOPutCh : public IOInstruction{
public:
    IOPutCh() = default;
    explicit IOPutCh(Value* op_value);
    //获取putch的定义语句
    static string getIODefine();
    string toLLVM() override;
    void generateMIPS() override;
};



#endif //IOPUTCH_H
