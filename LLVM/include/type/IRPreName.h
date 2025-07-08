//
// Created by PengXinyang on 24-11-21.
//

#ifndef IRPRENAME_H
#define IRPRENAME_H
#include<string>
#include <unordered_map>
using namespace std;

//前缀名枚举类，标记变量的前缀名
enum class IRPreType {
    GLOBAL_VAR,//全局变量前缀
    LOCAL_VAR,//局部变量前缀
    BLOCK,//块的前缀
    FUNCTION,//函数前缀
    CONST_STRING,//常量字符串的前缀
    Param,//形参
    ERROR//初始值赋这个，在代码生成中必须显式赋成其它值
};

class IRPreName {
private:
    IRPreType ir_pre_type = IRPreType::ERROR;
public:
    IRPreName() = default;
    explicit IRPreName(const IRPreType ir_pre_type) : ir_pre_type(ir_pre_type) {}
    // 获取对应的字符串前缀
    static string toLLVM(IRPreType preType);
    [[nodiscard]] string toLLVM() const {
        return toLLVM(ir_pre_type);
    }
};



#endif //IRPRENAME_H
