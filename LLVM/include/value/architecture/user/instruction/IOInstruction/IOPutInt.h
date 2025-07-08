//
// Created by PengXinyang on 24-11-23.
//

#ifndef IOPUTINT_H
#define IOPUTINT_H
#include "IOInstruction.h"


class IOPutInt :public IOInstruction{
public:
    IOPutInt() = default;
    explicit IOPutInt(Value* op_value);
    //获取putint的定义语句
    static string getIODefine();
    string toLLVM() override;
    void generateMIPS() override;
};



#endif //IOPUTINT_H
