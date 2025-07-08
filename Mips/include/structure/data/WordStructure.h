//
// Created by PengXinyang on 24-12-5.
//

#ifndef WORDSTRUCTURE_H
#define WORDSTRUCTURE_H
#include <vector>

#include "DataMipsStructure.h"

/**
* .data中的.word代码，定义整数
*/
class WordStructure :public DataMipsStructure{
private:
    vector<int> initValues;//存放初始值
public:
    WordStructure() = default;
    WordStructure(const string& label, const string& value,const vector<int>& initValues);
    string toMips() override;
};



#endif //WORDSTRUCTURE_H
