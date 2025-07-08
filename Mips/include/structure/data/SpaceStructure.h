//
// Created by PengXinyang on 24-12-5.
//

#ifndef SPACESTRUCTURE_H
#define SPACESTRUCTURE_H
#include "DataMipsStructure.h"

/**
* .space代码
*/
class SpaceStructure :public DataMipsStructure{
public:
    SpaceStructure() = default;
    SpaceStructure(const string& label, const string& value);
    string toMips() override;
};



#endif //SPACESTRUCTURE_H
