//
// Created by PengXinyang on 24-12-7.
//

#include "../../../../include/structure/text/MipsInstruction/Annotation.h"

#include "generate/MipsContent.h"

Annotation::Annotation(const string &annotation) {
    this->annotation = annotation;
    MipsContent::addTextSegment(this);
}

string Annotation::toMips() {
    return "\n\t#"+annotation;
}
