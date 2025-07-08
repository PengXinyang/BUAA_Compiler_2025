//
// Created by PengXinyang on 24-9-28.
//

#ifndef ERRORHANDLE_H
#define ERRORHANDLE_H
#include <map>
#include <vector>

#include "ErrorCategory.h"

class ErrorPrint {
public:
    //根据错误类别码和行号，输入至文件中
    static void printfError();
    static bool isError;
    //将文件出现的错误按照顺序加入map
    static void printError(ErrorCategory error_category,int line_num);
private:
    static vector<pair<int,char>> error_vector;
};



#endif //ERRORHANDLE_H
