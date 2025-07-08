//
// Created by PengXinyang on 24-11-24.
//

#ifndef GETELEMENTPTRINSTRUCTION_H
#define GETELEMENTPTRINSTRUCTION_H
#include "value/architecture/user/Instruction.h"

/*
* 数组，计算目标元素的位置
* 先只考虑一维数组
* <result> = getelementptr inbounds (<ty>, ptr <ptrval>{, <ty> <idx>}*)
* 第一个<ty>是第一个索引<idx>使用的基本类型
* 第二个<ty>*表示其后的基地址ptrval的类型。
* <ty> <idx>是第一组索引的类型和值， 可以出现多次，其后出现就是第二组、第三组等等索引的类型和值。
* 在本编译器中，索引是i32，实际上应该是i64
* 示例：
* @a = dso_local constant <{ i32, [19 x i32] }> <{ i32 2, [19 x i32] zeroinitializer }>
* %2 = load i32, i32* getelementptr inbounds ([20 x i32], [20 x i32]* bitcast (<{ i32, [19 x i32] }>* @a to [20 x i32]*), i64 0, i64 0)
* %5 = getelementptr inbounds [5 x i32], [5 x i32]* %2, i64 0, i64 1
*/
class GetelementptrInstruction : public Instruction{
public:
    GetelementptrInstruction() = default;

    /**
     * 构造函数
     * @param ir_type 数组的类型，int或者char数组
     * @param name 名称，或者说这个取地址要赋给哪个寄存器？
     * @param ptr_value 数组指针的类型
     * @param offset_value 要取哪个位置
     */
    GetelementptrInstruction(IRType* ir_type, const string& name, Value* ptr_value,Value* offset_value);
    string toLLVM() override;
    void generateMIPS() override;
};



#endif //GETELEMENTPTRINSTRUCTION_H
