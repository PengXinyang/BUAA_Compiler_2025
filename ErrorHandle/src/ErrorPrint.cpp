//
// Created by PengXinyang on 24-9-28.
//

#include "../include/ErrorPrint.h"

#include <algorithm>
#include<cstdio>

bool ErrorPrint::isError = false;
vector<pair<int,char>> ErrorPrint::error_vector;

void ErrorPrint::printfError() {
    if(isError) {
        FILE*file = fopen("./error.txt", "w");
        if (file == nullptr) {
            printf("File could not be opened\n");
        } else {
            sort(error_vector.begin(), error_vector.end());
            for(const auto&[line_num,error_type]:error_vector) {
                fprintf(file, "%d %c\n", line_num,error_type);
            }
        }
        fclose(file);
    }
}

void ErrorPrint::printError(ErrorCategory error_category, int line_num) {
    isError = true;
    pair<int,char> error;
    error.first = line_num;
    error.second = ErrorMap[error_category];
    int flag = 0;//标记本行有没有错误
    for(const auto&[line_num,error_type]:error_vector) {
        if(error.first == line_num) {
            flag = 1;//本行已经有错误，不再添加
        }
    }
    if(!flag) error_vector.push_back(error);
}
