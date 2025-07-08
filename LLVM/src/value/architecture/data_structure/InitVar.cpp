//
// Created by PengXinyang on 24-11-22.
//

#include "value/architecture/data_structure/InitVar.h"

#include <regex>
#include <sstream>

#include "../../../../include/type/irType/IRArray.h"

InitVar::InitVar(IRType* type, const vector<int> &initValues,const string& initString) {
    this->type = type;
    this->initValues = initValues;
    this->initString = regex_replace(initString,regex("\n"),"\\0A");
}

string InitVar::toLLVM() {
    if(initValues.empty()&& initString.empty()) {
        if(type->isArray()) {
            return type->toLLVM() + " zeroinitializer";
        }if(type->isInt() || type->isChar()) {
            return type->toLLVM() + " 0";
        }
    }else {
        if(type->isInt()) {
            return type->toLLVM() + " " + to_string(initValues[0]);
        }
        if(type->isChar()) {
            //不确定是否需要在这里考虑强转，先保留
            return type->toLLVM() + " " + to_string(initValues[0]);
        }
        if(type->isArray()) {
            //TODO 区分是int还是 char数组
            /**
             * 示例
             * int a[20]={2};
             * char c[] = "saadc";
             * char d[20]={'a','b'};
             * char m[20] = "abc";
             * int e[2] = {2,6};
             * @a = dso_local global <{ i32, [19 x i32] }> <{ i32 2, [19 x i32] zeroinitializer }>
             * @c = dso_local global [6 x i8] c"saadc\00"
             * @d = dso_local global <{ i8, i8, [18 x i8] }> <{ i8 97, i8 98, [18 x i8] zeroinitializer }>
             * @m = dso_local global [20 x i8] c"abc\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00"
             * @e = dso_local global [2 x i32] [i32 2, i32 6]
             */
            if(auto* array_type = dynamic_cast<IRArray*>(type)) {
                //定义一个变量，如果是int数组就是”i32“，如果是char数组就是”i8“
                string itype = "i32";
                if(array_type->get_dims().size()==1) {
                    //说明这是一维数组
                    //处理输出的字符串
                    ostringstream os;
                    //根据示例，我们必须先确定，赋值的个数是否与数组长度相同
                    const unsigned long long init_value_size = initValues.size();//赋值个数
                    const unsigned long long array_size = array_type->get_dims()[0];
                    int num = static_cast<int>(array_size-init_value_size);
                    //如果数组是int类型
                    if(array_type->get_element_type()->isInt()) {
                        itype = "i32";
                        if(init_value_size < array_size) {
                            //说明赋值元素少，采用示例"a"的赋值方式
                            //TODO: 数组类型转换，[20 x i32] 和 <{i32, i32, [18 x i32]}>相互转化
                            /*os<<"<{ ";
                            for(int i = 0; i < init_value_size; i++) {
                                os<<itype<<", ";
                            }
                            //对于没有赋值到的，统一写例如[19 x i32]之类
                            os<<array_type->toLLVM(num);
                            os<<" }> <{ ";
                            for(int i = 0; i < init_value_size; i++) {
                                os<<itype<<" "<<initValues[i]<<", ";
                            }
                            os<<array_type->toLLVM(num);
                            os<<" zeroinitializer }>";*/
                            os << array_type->toLLVM() << " [";
                            for (size_t i = 0; i < init_value_size; ++i) {
                                os << "i32 " << initValues[i] <<", ";
                            }
                            //剩余部分填0
                            for(size_t i = init_value_size; i < array_size; ++i) {
                                os << "i32 0";
                                if(i+1<array_size) {
                                    os<<", ";
                                }
                            }
                            os << "]";
                        }
                        else {
                            //赋值元素不缺少，仿照示例的数组e
                            //如果数组定义时没有明确写出来有几个元素，就要将initValues长度作为数组元素个数
                            if(array_size == 0) os << array_type->toLLVM(static_cast<int>(init_value_size)) << " [";
                            else os << array_type->toLLVM() << " [";
                            for (size_t i = 0; i < init_value_size; ++i) {
                                os << "i32 " << initValues[i];
                                if (i + 1 < init_value_size) {
                                    os << ", ";
                                }
                            }
                            os << "]";
                        }
                    }else if(array_type->get_element_type()->isChar()) {
                        itype = "i8";
                        //如果赋值是字符串，单独在这里处理
                        if(!initString.empty()) {
                            //按照字符串处理
                            //首先，去除首尾引号
                            initString = initString.substr(1,initString.size()-2);
                            //要分为数组是否显式定义了元素个数
                            if(array_size==0) {
                                //没有显式定义，字符串长度+1就是要分配的数组空间
                                os<<array_type->toLLVM(static_cast<int>(initString.length())+1);
                                os<<" c\""<<initString<<"\\00\"";
                            }else {
                                //有自己的数组长度定义
                                os<<array_type->toLLVM();
                                os<<" c\""<<initString;
                                for(int i=0;i<array_size-initString.length();++i) {
                                    os<<"\\00";
                                }
                                os<<"\"";
                            }
                        }
                        else {
                            os << array_type->toLLVM() << " [";
                            for (size_t i = 0; i < init_value_size; ++i) {
                                os << "i8 " << initValues[i];
                                if(i+1<init_value_size) {
                                    os<<", ";
                                }
                            }
                            if(init_value_size < array_size) {
                                //剩余部分填0
                                os<<", ";
                                for(size_t i = init_value_size; i < array_size; ++i) {
                                    os << "i8 0";
                                    if(i+1<array_size) {
                                        os<<", ";
                                    }
                                }
                            }
                            os << "]";
                        }
                    }
                    return os.str();
                }
                //这里用于处理未来更高维数组的情况
            }else {
                //是数组类型却不能强制转换成IRArray，报错
                printf("Error in InitVar::toLLVM()\n");
            }
        }
    }
    return "";
}
