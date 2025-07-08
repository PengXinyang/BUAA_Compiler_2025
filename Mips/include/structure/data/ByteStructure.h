//
// Created by PengXinyang on 24-12-5.
//

#ifndef BTYPESTRUCTURE_H
#define BTYPESTRUCTURE_H
#include <vector>

#include "DataMipsStructure.h"

/**
* char 类型使用的byte定义
*/
class ByteStructure :public DataMipsStructure{
private:
    vector<int> initValues;//存放初始值
public:
    ByteStructure() = default;
    ByteStructure(const string& label, const string& value, const vector<int>& initValues);
    string toMips() override;
};



#endif //BTYPESTRUCTURE_H
