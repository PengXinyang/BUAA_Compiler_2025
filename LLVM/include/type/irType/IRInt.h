//
// Created by PengXinyang on 24-11-21.
//

#ifndef IRINT_H
#define IRINT_H
#include "../IRType.h"


class IRInt :public IRType{
public:
    bool isInt() override;
    string toLLVM() override;
};



#endif //IRINT_H
