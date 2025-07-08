//
// Created by PengXinyang on 24-11-22.
//

#include "../../../../include/value/architecture/user/GlobalVariable.h"

#include "optimize/LLVMOptimizerInit.h"
#include "structure/data/AsciizStructure.h"
#include "structure/data/ByteStructure.h"
#include "structure/data/SpaceStructure.h"
#include "structure/data/WordStructure.h"
#include "type/IRName.h"
#include "type/irType/IRArray.h"

GlobalVariable::GlobalVariable(IRType* ir_type, const string& name, InitVar* init_var, bool is_const_array):GlobalValue(ir_type,name) {
    initVar = init_var;
    this->is_const_array = is_const_array;
    if(!LLVMOptimizerInit::isOptimize()) {
        //如果不需要优化，直接添加
        IRName::addGlobalVariable(this);
    }
}

string GlobalVariable::toLLVM() {
    if(is_const_array) return value_name + " = dso_local constant " + initVar->toLLVM();
    return value_name + " = dso_local global " + initVar->toLLVM();
}

void GlobalVariable::generateMIPS() {
    IRType* ir_type = value_type;
    if(instanceof<IRPointer>(value_type)) {
        ir_type = dynamic_cast<IRPointer*>(value_type)->ir_type();
    }
    if(ir_type->isInt()) {
        //如果是int类型，则赋第一个值
        const int num = initVar->getInitValues().empty() ? 0 : initVar->getInitValues()[0];
        new WordStructure(value_name.substr(1),to_string(num),vector<int>());
    }else if(ir_type->isChar()) {
        //如果是char类型，则
        const int num = initVar->getInitValues().empty() ? 0 : initVar->getInitValues()[0];
        new ByteStructure(value_name.substr(1),to_string(num),vector<int>());
        //new WordStructure(value_name.substr(1),to_string(num),vector<int>());
    }else {
        //说明是数组
        //TODO:数组应当使用space
        if(ir_type->isArray()) {
            if(dynamic_cast<IRArray*>(ir_type)->get_element_type()->isInt()) {
                //是int数组
                vector<int> vec = initVar->getInitValues();
                //如果初始化值为空，用space赋值
                if(vec.empty()) {
                    new SpaceStructure(value_name.substr(1),to_string(dynamic_cast<IRArray*>(ir_type)->calculate_dims()*4));
                }
                else new WordStructure(value_name.substr(1),to_string(dynamic_cast<IRArray*>(ir_type)->calculate_dims()),vec);
            }else if(dynamic_cast<IRArray*>(ir_type)->get_element_type()->isChar()) {
                //是char数组
                const string initString = initVar->getInitString();
                if(!initString.empty()) {
                    //new AsciizStructure(value_name.substr(1),initString);
                    vector<int> vec;
                    vec.reserve(initString.length());
                    for(int i = 0; i < initString.length(); i++) {
                        char c = initString[i];
                        if(c=='\\' && i+1<initString.length()) {
                            //说明后面可能有转义字符
                            switch(initString[i+1]) {
                                case 'a': c = 7; i++;break;
                                case 'b': c = 8;i++;break;
                                case 't': c = 9;i++;break;
                                case 'n': c = 10;i++;break;
                                case 'v': c = 11;i++;break;
                                case 'f': c = 12;i++;break;
                                case '\"': c = 34;i++;break;
                                case '\'': c = 39;i++;break;
                                case '\\': c = 92;i++;break;
                                case '0': c = 0;i++;break;
                                default: c = 92;
                            }
                        }
                        vec.push_back(c);
                    }
                    new ByteStructure(value_name.substr(1),to_string(dynamic_cast<IRArray*>(ir_type)->calculate_dims()),vec);
                }
                else {
                    vector<int> vec = initVar->getInitValues();
                    //如果初始化值为空，用space赋值
                    if(vec.empty()) {
                        new SpaceStructure(value_name.substr(1),to_string(dynamic_cast<IRArray*>(ir_type)->calculate_dims()));
                    }
                    else new ByteStructure(value_name.substr(1),to_string(dynamic_cast<IRArray*>(ir_type)->calculate_dims()),vec);
                }
            }
        }
    }
}
