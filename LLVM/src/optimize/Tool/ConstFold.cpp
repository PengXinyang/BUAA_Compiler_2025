//
// Created by PengXinyang on 24-12-12.
//

#include "optimize/Tool/ConstFold.h"

#include "type/irType/IRInt.h"
#include "value/architecture/ConstValue.h"

void ConstFold::handleCalculateInstruction(BasicBlock *basic_block) {
    vector<Instruction *>& instructions = basic_block->getInstructions();
    for(auto it = instructions.begin(); it != instructions.end();) {
        Instruction *instruction = *it;
        //如果是计算指令，直接进入下面三个部分优化
        if(instanceof<CalculateInstruction>(instruction)) {
            auto *calculateInstruction = dynamic_cast<CalculateInstruction *>(instruction);
            Value* new_const = nullptr;
            if(calculateInstruction->getConstNum()==0) {
                //说明两个操作数都不是常量
                new_const = handle0(calculateInstruction);
            }else if(calculateInstruction->getConstNum()==1) {
                new_const = handle1(calculateInstruction);
            }else if(calculateInstruction->getConstNum()==2) {
                new_const = handle2(calculateInstruction);
            }
            if(new_const) {
                calculateInstruction->replaceAllUser(new_const);
                //删除并更新迭代器
                auto it_new = instructions.erase(it);
                //可以考虑删除这个指令？
                //delete *it;
                it = it_new;
            }else {
                ++it;
            }
        }else {
            ++it;
        }
        //如果是强制转换指令，在llvm的计算中，所有指令强制从char转int，
        //所以直接把这个常量值取出来即可
    }
}

Value *ConstFold::handle0(CalculateInstruction *calculate_instruction) {
    ConstValue *new_const = nullptr;
    if(calculate_instruction->opValueChain[0] == calculate_instruction->opValueChain[1]) {
        if(calculate_instruction->getInstructionType()=="sub" || calculate_instruction->getInstructionType()=="srem") {
            new_const = new ConstValue(new IRInt(),"0");
        }else if(calculate_instruction->getInstructionType()=="sdiv") {
            new_const = new ConstValue(new IRInt(),"1");
        }
    }
    return new_const;
}

Value *ConstFold::handle1(CalculateInstruction *calculate_instruction) {
    if(instanceof<ConstValue>(calculate_instruction->opValueChain[0])) {
        //说明第一个是常数，主要优化0+和0*，1*的情况
        long long val = dynamic_cast<ConstValue *>(calculate_instruction->opValueChain[0])->getNum();
        if(val == 0) {
            if(calculate_instruction->getInstructionType()=="add") {
                return calculate_instruction->opValueChain[1];
            }
            if(calculate_instruction->getInstructionType()=="mul" || calculate_instruction->getInstructionType()=="srem" || calculate_instruction->getInstructionType()=="sdiv") {
                //0*,0/,0%都是0
                return new ConstValue(new IRInt(),"0");
            }
        }else if(val == 1) {
            if(calculate_instruction->getInstructionType()=="mul") {
                return calculate_instruction->opValueChain[1];
            }
        }
    }else {
        //说明第二个是常量
        long long val = dynamic_cast<ConstValue *>(calculate_instruction->opValueChain[1])->getNum();
        if(val == 0) {
            if(calculate_instruction->getInstructionType()=="add" || calculate_instruction->getInstructionType()=="aub") {
                //+0, -0返回第一个操作数即可
                return calculate_instruction->opValueChain[0];
            }
            if(calculate_instruction->getInstructionType()=="mul") {
                //*0为0
                return new ConstValue(new IRInt(),"0");
            }
        }else if(val == 1) {
            if(calculate_instruction->getInstructionType()=="mul" || calculate_instruction->getInstructionType()=="sdiv") {
                //*1,/1返回第一个值
                return calculate_instruction->opValueChain[0];
            }if(calculate_instruction->getInstructionType()=="srem") {
                //%1就是0
                return new ConstValue(new IRInt(),"0");
            }
        }
    }
    return nullptr;
}

Value *ConstFold::handle2(CalculateInstruction *calculate_instruction) {
    long long op_value0 = dynamic_cast<ConstValue*>(calculate_instruction->opValueChain[0])->getNum();
    long long op_value1 = dynamic_cast<ConstValue*>(calculate_instruction->opValueChain[1])->getNum();
    if(calculate_instruction->getInstructionType()=="add") {
        return new ConstValue(new IRInt(), to_string(op_value0+op_value1));
    }
    if(calculate_instruction->getInstructionType()=="sub") {
        return new ConstValue(new IRInt(), to_string(op_value0-op_value1));
    }
    if(calculate_instruction->getInstructionType()=="mul") {
        return new ConstValue(new IRInt(), to_string(op_value0*op_value1));
    }
    //错误处理，如果op1是0并且涉及除法和取模，那么置为 -1
    if(calculate_instruction->getInstructionType()=="sdiv") {
        if(op_value1==0) op_value1 = -1;
        return new ConstValue(new IRInt(), to_string(op_value0/op_value1));
    }
    if(calculate_instruction->getInstructionType()=="srem") {
        if(op_value1==0) op_value1 = -1;
        return new ConstValue(new IRInt(), to_string(op_value0%op_value1));
    }
    if(calculate_instruction->getInstructionType()=="and") {
        return new ConstValue(new IRInt(), to_string(op_value0&op_value1));
    }
    if(calculate_instruction->getInstructionType()=="or") {
        return new ConstValue(new IRInt(), to_string(op_value0|op_value1));
    }
    return nullptr;
}
