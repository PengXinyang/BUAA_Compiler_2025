//
// Created by PengXinyang on 24-12-4.
//

#ifndef MIPSGENERATE_H
#define MIPSGENERATE_H
#include "generate/MipsContent.h"
#include "../../LLVM/include/value/architecture/Module.h"

/**
* 生成MIPS代码
*/
class MipsGenerate {
private:
    Module*module = nullptr;
    MipsContent*mips_content = nullptr;
    static MipsGenerate* instance;
public:
    MipsGenerate() = default;
    explicit MipsGenerate(Module* module);
    static MipsGenerate* getInstance(Module* module);
    //生成Mips代码
    void generateMips();
    //输出Mips
    void printMips();
};



#endif //MIPSGENERATE_H
