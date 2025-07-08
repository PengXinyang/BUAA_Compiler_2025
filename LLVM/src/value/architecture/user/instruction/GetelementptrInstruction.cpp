//
// Created by PengXinyang on 24-11-24.
//

#include "../../../../../include/value/architecture/user/instruction/GetelementptrInstruction.h"

#include "Register/RegisterController.h"
#include "Register/RegisterTool.h"
#include "structure/Register.h"
#include "type/irType/IRArray.h"
#include "type/irType/IRPointer.h"

GetelementptrInstruction::GetelementptrInstruction(IRType* ir_type, const string& name, Value *ptr_value, Value *offset_value)
    :Instruction(new IRPointer(ir_type),name, "getelementptr"){
    addOpValue(ptr_value);
    addOpValue(offset_value);
}

string GetelementptrInstruction::toLLVM() {
    //数组形式：
    //%5 = getelementptr inbounds [5 x i32], [5 x i32]* %2, i32 0, i32 1
    //指针形式：（主要用于函数定义中，形参是数组，表现是指针
    //%7 = getelementptr inbounds i32, i32* %6, i32 2
    ostringstream oss;
    //第一个操作数就是%2
    //TODO:NULL
    auto* ptr_value_type = dynamic_cast<IRPointer*>(opValueChain[0]->value_type);
    IRType* ir_type = opValueChain[0]->value_type;
    //TODO：特判，需要修正
    if(!ptr_value_type) ptr_value_type = new IRPointer(ir_type);
    else ir_type = ptr_value_type->ir_type();
    //%5 = getelementptr inbounds
    oss<<value_name<<" = getelementptr inbounds ";
    //[5 x i32] 或 i32
    oss<<ir_type->toLLVM()<<", ";
    //[5 x i32]* 或 i32*
    oss<<ptr_value_type->toLLVM()<<" ";
    //%2,
    oss<<opValueChain[0]->value_name<<", ";
    //如果是数组，要加i32 0；如果是指针，则不加
    if(ir_type->isArray()) {
        oss<<"i32 0, ";
    }
    //TODO:如果中括号内是常量值，比如a[3]，可能需要修改，最后加的不是name，而是值
    //ConstValue的num字段？
    oss<<opValueChain[1]->value_type->toLLVM()<<" "<<opValueChain[1]->value_name;
    return oss.str();
}

void GetelementptrInstruction::generateMIPS() {
    Instruction::generateMIPS();
    // 首先在寄存器控制器中查找当前指令对应的寄存器，如果为null则使用t8寄存器
    Register* rd = RegisterController::getRegister(this);
    if(!rd) rd = Register::getRegister(RegisterName::$t8);
    //同理，也需要查找当前指令的操作数对应的寄存器，如果rs为null则使用t8寄存器,
    //如果rt为null则使用t9寄存器,当然这里由于我们的操作数是一个指针，所以我们还需要
    //从指针中获取对应的地址
    Register* pointer_register = RegisterController::getRegister(opValueChain[0]);
    Register* offset_register = RegisterController::getRegister(opValueChain[1]);
    pointer_register = RegisterTool::loadPointerValue(opValueChain[0],pointer_register,Register::getRegister(RegisterName::$t8));
    //value_type一定是指针类型，并且根据调用的方式，一定是int/char/void复合成指针类型
    //这种方式可以直接获得元素的真正类型，当然是外面套了一层pointer
    IRType* ir_type = value_type;
    if(instanceof<IRArray>(ir_type)) {
        ir_type = dynamic_cast<IRArray*>(ir_type)->get_element_type();
    }
    if(instanceof<IRPointer>(ir_type)) {
        ir_type = dynamic_cast<IRPointer*>(ir_type)->ir_type();
    }
    offset_register = RegisterTool::loadMemoryOffset(opValueChain[1],rd,Register::getRegister(RegisterName::$t9),pointer_register,offset_register,ir_type);
    //如果之前使用了默认寄存器，则需要重新分配地址，并在寄存器控制器中更新对应的寄存器
    RegisterTool::reAllocaRegister(this,rd);
}
