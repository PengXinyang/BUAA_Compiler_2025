//
// Created by PengXinyang on 24-12-5.
//

#include "../../../include/structure/data/SpaceStructure.h"

SpaceStructure::SpaceStructure(const string& label, const string& value)
    : DataMipsStructure(label, value){

}

string SpaceStructure::toMips() {
    return label + ": .space " + value;
}
