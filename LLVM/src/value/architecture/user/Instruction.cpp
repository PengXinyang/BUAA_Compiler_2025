//
// Created by PengXinyang on 24-11-23.
//

#include "../../../../include/value/architecture/user/Instruction.h"

#include "optimize/LLVMOptimizerInit.h"
#include "optimize/MemToReg.h"
#include "structure/text/MipsInstruction/Annotation.h"
#include "type/IRName.h"
#include "value/architecture/user/instruction/AllocaInstruction.h"
#include "value/architecture/user/instruction/CalculateInstruction.h"
#include "value/architecture/user/instruction/CallInstruction.h"
#include "value/architecture/user/instruction/GetelementptrInstruction.h"
#include "value/architecture/user/instruction/IcmpInstruction.h"
#include "value/architecture/user/instruction/LoadInstruction.h"
#include "value/architecture/user/instruction/PhiInstruction.h"
#include "value/architecture/user/instruction/TruncInstruction.h"
#include "value/architecture/user/instruction/ZextInstruction.h"
#include "value/architecture/user/instruction/IOInstruction/IOGetChar.h"
#include "value/architecture/user/instruction/IOInstruction/IOGetInt.h"

Instruction::Instruction(IRType* ir_type, const string& name, const string &instructionType):User(ir_type,name) {
    this->instructionType = instructionType;
    if(!LLVMOptimizerInit::isOptimize()) {
        IRName::addInstruction(this);
    }
}

void Instruction::generateMIPS() {
    //将指令作为注释生成
    new Annotation(toLLVM());
}

void Instruction::insertPhiInstruction() {
    MemToReg::resetByInstruction(this);
    MemToReg::insertPhiInstruction();
}

void Instruction::generateUseDef() {
    unordered_set<Value*>& use = blockParent->getUseSet();
    unordered_set<Value*>& def = blockParent->getDefSet();
    for(auto op:opValueChain) {
        if(def.find(op) == def.end() && (instanceof<Instruction>(op) || instanceof<GlobalVariable>(op) || instanceof<Param>(op))) {
            //说明op既不在定义链，同时也属于指令/全局变量/参数，应当加入使用链
            use.insert(op);
        }
    }
    //如果使用链没有自己，那么自己就是属于定义链
    if(use.find(this)==use.end() && this->isUseful()) {
        def.insert(this);
    }
}

bool Instruction::isUseful() {
    int index = 0;
    index = instanceof<AllocaInstruction>(this)
    + instanceof<CalculateInstruction>(this)
    + (instanceof<CallInstruction>(this) && !dynamic_cast<CallInstruction*>(this)->value_type->isVoid())
    + instanceof<IOGetInt>(this)
    + instanceof<IOGetChar>(this)
    + instanceof<PhiInstruction>(this)
    + instanceof<GetelementptrInstruction>(this)
    + instanceof<IcmpInstruction>(this)
    + instanceof<LoadInstruction>(this)
    + instanceof<ZextInstruction>(this)
    + instanceof<TruncInstruction>(this);
    return index != 0;
}
