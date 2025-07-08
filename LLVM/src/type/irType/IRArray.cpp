//
// Created by PengXinyang on 24-11-21.
//

#include "../../../include/type/irType/IRArray.h"

IRArray::IRArray(IRType* type, const vector<int> &dims) {
    elementType = type;
    if(dims.empty()) {
        this->dims = vector<int>(1, 0);
    }
    else this->dims = dims;
}
IRArray::IRArray(IRType *type, int dim) {
    elementType = type;
    this->dims.push_back(dim);
}

bool IRArray::isArray() {
    return true;
}

int IRArray::calculate_dims() {
    int res = 1;
    for (const int dim : dims) {
        res *= dim;
    }
    return res;
}

string IRArray::toLLVM() {
    if(dims.size() == 1) {
        return "[" + to_string(dims[0]) + " x " + elementType->toLLVM() + "]";
    }
    //如果未来考多维数组，再添加
    return "";
}

string IRArray::toLLVM(int dim) {
    return "[" + to_string(dim) + " x " + elementType->toLLVM() + "]";
}
