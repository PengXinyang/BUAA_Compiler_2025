//
// Created by PengXinyang on 24-12-5.
//

#include "../../../../include/structure/text/MipsInstruction/SyscallInstruction.h"

#include "generate/MipsContent.h"

SyscallInstruction::SyscallInstruction() {
    MipsContent::addTextSegment(this);
}
string SyscallInstruction::toMips() {
    return "syscall";
}
