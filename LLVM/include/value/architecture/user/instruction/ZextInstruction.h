//
// Created by PengXinyang on 24-11-24.
//

#ifndef ZEXTINSTRUCTION_H
#define ZEXTINSTRUCTION_H
#include "../Instruction.h"

/*
* 零扩展指令，当 char 类型值赋给 int 类型变量时，需要进行扩展
* <result> = zext <ty> <value> to <ty2>
* 示例：
* %12 = zext i8 %11 to i32
*/
class ZextInstruction : public Instruction{
private:
    IRType* final_type = nullptr;//要转换成的type
public:
    ZextInstruction() = default;
    ZextInstruction(const string& name, Value* value, IRType* final_type);
    string toLLVM() override;
    void generateMIPS() override;
};



#endif //ZEXTINSTRUCTION_H
