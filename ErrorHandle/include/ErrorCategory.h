//
// Created by PengXinyang on 24-9-28.
//

#ifndef ERRORCATEGORY_H
#define ERRORCATEGORY_H
#include<map>
#include <unordered_map>
using namespace std;
//错误类别码
enum  class ErrorCategory {
    illegal_symbol,//a，非法符号
    redefine,//b,名字重定义
    undefined,//c,未定义名字
    param_num_not_match,//d,参数个数不匹配
    param_type_not_match,//e,参数类型不匹配
    return_not_match,//f,无返回值函数返回值不匹配
    return_lack,//g,缺少返回值
    const_error,//h,不能改变常量的值
    semicolon_lack,//i,缺少分号
    r_parenthesis_lack,//j,缺少右小括号
    r_bracket_lack,//k,缺少右中括号
    format_num_not_match,//l,printf格式字符个数不匹配
    break_continue_not_loop//m,非循环使用break和continue
};
inline map<ErrorCategory,char> ErrorMap={
    {ErrorCategory::illegal_symbol,'a'},
    {ErrorCategory::redefine,'b'},
    {ErrorCategory::undefined,'c'},
    {ErrorCategory::param_num_not_match,'d'},
    {ErrorCategory::param_type_not_match,'e'},
    {ErrorCategory::return_not_match,'f'},
    {ErrorCategory::return_lack,'g'},
    {ErrorCategory::const_error,'h'},
    {ErrorCategory::semicolon_lack,'i'},
    {ErrorCategory::r_parenthesis_lack,'j'},
    {ErrorCategory::r_bracket_lack,'k'},
    {ErrorCategory::format_num_not_match,'l'},
    {ErrorCategory::break_continue_not_loop,'m'}
};

#endif //ERRORCATEGORY_H
