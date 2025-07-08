//
// Created by PengXinyang on 24-12-4.
//

#ifndef DATAMIPSSTRUCTURE_H
#define DATAMIPSSTRUCTURE_H
#include "structure/MipsStructure.h"

/**
* DataMipsStructure 是.data段中的汇编代码
*/
class DataMipsStructure :public MipsStructure{
public:
    string label;//汇编代码的标签
    string value;//汇编的值
    DataMipsStructure() = default;
    DataMipsStructure(const string &label, const string &value);
};



#endif //DATAMIPSSTRUCTURE_H
