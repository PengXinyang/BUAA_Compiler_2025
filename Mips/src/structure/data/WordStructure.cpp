//
// Created by PengXinyang on 24-12-5.
//

#include "../../../include/structure/data/WordStructure.h"

#include <sstream>

WordStructure::WordStructure(const string& label, const string& value, const vector<int>& initValues)
    : DataMipsStructure(label, value){
    this->initValues = initValues;
}

string WordStructure::toMips() {
    if (initValues.empty()) {
        //没有初始值
        return label + ": .word " + value;
    }
    int offset = stoi(value) - static_cast<int>(initValues.size());
    ostringstream oss;
    oss<<label + ": .word ";
    for (int i = 0; i < initValues.size(); i++) {
        oss << initValues[i];
        if(i+1<stoi(value)) {
            oss<<", ";
        }
    }
    for(int i = 0;i<offset;i++) {
        oss<<"0";
        if(i+1<offset) {
            oss<<", ";
        }
    }
    return oss.str();
}
