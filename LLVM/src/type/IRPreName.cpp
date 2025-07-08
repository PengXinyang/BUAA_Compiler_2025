//
// Created by PengXinyang on 24-11-21.
//

#include "../../include/type/IRPreName.h"

string IRPreName::toLLVM(const IRPreType preType) {
    static const unordered_map<IRPreType, string> preTypeMap = {
        {IRPreType::GLOBAL_VAR, "@"},
        {IRPreType::LOCAL_VAR, "%var_"},
        {IRPreType::BLOCK, "label_"},
        {IRPreType::FUNCTION, "@"},
        {IRPreType::CONST_STRING, "@.str."},
        {IRPreType::Param,"%param_"}
    };
    if (const auto it = preTypeMap.find(preType); it != preTypeMap.end()) {
        return it->second;
    }
    return ""; // 如果没有匹配项，返回空字符串
}