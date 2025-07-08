//
// Created by PengXinyang on 24-12-4.
//

#include "MipsGenerate.h"

#include <fstream>
#include <iostream>

MipsGenerate* MipsGenerate::instance = nullptr;
MipsGenerate::MipsGenerate(Module* module) {
    this->module = module;
    mips_content = new MipsContent();
}
MipsGenerate *MipsGenerate::getInstance(Module* module) {
    if (!instance) {
        instance = new MipsGenerate(module);
    }
    return instance;
}

void MipsGenerate::generateMips() {
    //可能涉及到一些初始化
    //new AssemblyUnit().init();
    module->generateMIPS();
}

void MipsGenerate::printMips() {
    ofstream outFile("mips.txt");
    // 检查文件是否成功打开
    if (!outFile) {
        cerr << "Error: Unable to open file!" << endl;
    }
    // 将字符串写入文件
    outFile << mips_content->toMips();
    // 关闭文件
    outFile.close();
}
