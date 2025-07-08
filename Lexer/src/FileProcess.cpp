//
// Created by PengXinyang on 24-9-22.
//

#include "../include/FileProcess.h"
#include<sstream>
#include<fstream>
//从文件读入到string里
string FileProcess::readFileIntoString(const char * filename){
    ifstream ifile(filename);
    //将文件读入到ostringstream对象buf中
    ostringstream buf;
    char ch;
    while(buf&&ifile.get(ch))
        buf.put(ch);
    //返回与流对象buf关联的字符串
    return buf.str();
}