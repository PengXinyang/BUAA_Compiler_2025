//
// Created by PengXinyang on 24-11-21.
//

#ifndef IRARRAY_H
#define IRARRAY_H
#include <vector>

#include "../IRType.h"


class IRArray :public IRType{
private:
    vector<int> dims;//每一维数组的元素个数，虽然文法只有一维数组，但是担心期末考多维数组,比如A[4][5],则记录dims[0]=4,dims[1]=5
    IRType* elementType{};//数组元素的类型
public:
    IRArray() = default;
    IRArray(IRType* type, const vector<int> &dims);
    IRArray(IRType* type, int dim);
    [[nodiscard]] vector<int>& get_dims(){
        return dims;
    }

    [[nodiscard]] IRType* get_element_type() const {
        return elementType;
    }
    bool isArray() override;
    int calculate_dims();
    string toLLVM() override;
    //按照数组长度是dim输出
    string toLLVM(int dim);
};



#endif //IRARRAY_H
