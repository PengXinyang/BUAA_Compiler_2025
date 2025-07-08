//
// Created by PengXinyang on 24-12-7.
//

#ifndef ANNOTATION_H
#define ANNOTATION_H
#include "structure/MipsStructure.h"

/**
* 生成注释
*/
class Annotation :public MipsStructure{
private:
    string annotation;
public:
    Annotation() = default;
    Annotation(const string &annotation);
    string toMips() override;
};



#endif //ANNOTATION_H
