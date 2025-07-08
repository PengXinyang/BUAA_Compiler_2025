//
// Created by PengXinyang on 24-12-5.
//

#ifndef ASCIIZSTRUCTURE_H
#define ASCIIZSTRUCTURE_H
#include "DataMipsStructure.h"

/**
* .data段中的.asciiz汇编代码
* 用于常量字符串的定义
*/
class AsciizStructure :public DataMipsStructure{
public:
    AsciizStructure() = default;
    AsciizStructure(const string& label, const string& value);
    string toMips() override;
};



#endif //ASCIIZSTRUCTURE_H
