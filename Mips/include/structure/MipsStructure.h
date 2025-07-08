//
// Created by PengXinyang on 24-12-4.
//

#ifndef MIPSSTRUCTURE_H
#define MIPSSTRUCTURE_H
#include <string>
using namespace std;
/**
* mips结构类，基类，所有mips指令继承于这个结构
*/
class MipsStructure {
public:
    MipsStructure() = default;
    virtual ~MipsStructure() = default;
    virtual string toMips(){return "";}
};



#endif //MIPSSTRUCTURE_H
