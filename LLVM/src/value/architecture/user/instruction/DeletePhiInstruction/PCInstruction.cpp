//
// Created by PengXinyang on 24-12-18.
//

#include "value/architecture/user/instruction/DeletePhiInstruction/PCInstruction.h"

#include "type/irType/IRVoid.h"

PCInstruction::PCInstruction(const string& name) :Instruction(new IRVoid(),name,"PC"){

}

vector<Value *> &PCInstruction::getDestination() {
    return destination;
}
vector<Value *> &PCInstruction::getSource() {
    return source;
}
void PCInstruction::setSrcDst(Value *src, Value *dst) {
    source.push_back(src);
    destination.push_back(dst);
}
