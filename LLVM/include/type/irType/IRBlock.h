//
// Created by PengXinyang on 24-11-21.
//

#ifndef IRBLOCK_H
#define IRBLOCK_H
#include <utility>

#include "../IRType.h"


//主要用于生成结构块类型，如Function，BasicBlock等
class IRBlock :public IRType{
private:
    string block_name;//函数/块名
public:
    IRBlock()=default;
    explicit IRBlock(const string& block_name) {
        this->block_name = block_name;
    }
    bool isBlock() override;
    string toLLVM() override;
};



#endif //IRBLOCK_H
