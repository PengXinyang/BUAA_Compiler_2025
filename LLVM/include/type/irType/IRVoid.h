//
// Created by PengXinyang on 24-11-21.
//

#ifndef IRVOID_H
#define IRVOID_H
#include <string>
using namespace std;
#include "../IRType.h"

//void类型
class IRVoid : public IRType{
public:
    bool isVoid() override;
    string toLLVM() override;
};



#endif //IRVOID_H
