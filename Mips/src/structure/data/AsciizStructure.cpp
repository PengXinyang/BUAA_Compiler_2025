//
// Created by PengXinyang on 24-12-5.
//

#include "../../../include/structure/data/AsciizStructure.h"
AsciizStructure::AsciizStructure(const string& label, const string& value)
    :DataMipsStructure(label, value){

}

string AsciizStructure::toMips() {
    string escaped_value = value;
    // 替换所有的换行符 "\n" 为 "\\n"
    size_t pos = 0;
    while ((pos = escaped_value.find('\n', pos)) != string::npos) {
        escaped_value.replace(pos, 1, "\\n");
        pos += 2; // 跳过已替换的 "\\n"
    }

    return label + ": .asciiz \"" + escaped_value + "\"";;
}
