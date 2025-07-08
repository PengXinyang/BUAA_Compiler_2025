//
// Created by PengXinyang on 24-11-28.
//

#ifndef IRBOOL_H
#define IRBOOL_H
#include "type/IRType.h"


class IRBool :public IRType{
public:
    bool isBool() override;
    string toLLVM() override;
};



#endif //IRBOOL_H
