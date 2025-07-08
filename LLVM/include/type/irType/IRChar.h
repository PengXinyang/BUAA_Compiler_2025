//
// Created by PengXinyang on 24-11-21.
//

#ifndef IRCHAR_H
#define IRCHAR_H
#include <string>

#include "../IRType.h"


class IRChar :public IRType{
public:
    bool isChar() override;
    string toLLVM() override;
};



#endif //IRCHAR_H
