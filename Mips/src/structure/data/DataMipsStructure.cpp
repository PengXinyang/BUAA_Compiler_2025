//
// Created by PengXinyang on 24-12-4.
//

#include "structure/data/DataMipsStructure.h"

#include "generate/MipsContent.h"

DataMipsStructure::DataMipsStructure(const string &label, const string &value) {
    this->label = label;
    this->value = value;
    MipsContent::addDataSegment(this);
}
