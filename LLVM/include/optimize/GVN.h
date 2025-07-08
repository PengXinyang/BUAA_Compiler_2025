//
// Created by PengXinyang on 24-12-12.
//

#ifndef GVN_H
#define GVN_H
#include "value/architecture/Module.h"

/**
* 全局变量优化，事实上也包括了LVN的常量折叠
*/
class GVN {
public:
    //运行常量折叠
    static void runConstFold(Module* module);
    void ConstFoldImpl(BasicBlock* basic_block);
};



#endif //GVN_H
