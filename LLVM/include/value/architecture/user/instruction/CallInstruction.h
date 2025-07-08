//
// Created by PengXinyang on 24-11-24.
//

#ifndef CALLINSTRUCTION_H
#define CALLINSTRUCTION_H
#include "value/architecture/user/Function.h"
#include "value/architecture/user/Instruction.h"

/*
* 函数调用指令
* <result> = call [ret attrs] <ty> <name>(<...args>)
* 示例：
* %17 = call i32 @f(i32 %16)
* call void @g(i32 %19)
*/
class CallInstruction :public Instruction{
private:
    Function* call_function = nullptr;//调用的目标函数指针
public:
    CallInstruction() = default;
    //初始化函数调用指令，其中args表示调用的实参数，name表示赋值的变量名称，例如%17
    CallInstruction(const string& name, Function* call_function, vector<Value*>& args);
    [[nodiscard]] Function* getCallFunction() const;
    string toLLVM() override;
    void generateMIPS() override;
};



#endif //CALLINSTRUCTION_H
