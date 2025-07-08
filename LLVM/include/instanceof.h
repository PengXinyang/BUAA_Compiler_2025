//
// Created by PengXinyang on 24-11-23.
//

#ifndef INSTANCEOF_H
#define INSTANCEOF_H

template<typename ClassType, typename ObjectType>
bool instanceof(ObjectType* object) {
    return dynamic_cast<ClassType*>(object) != nullptr;
}

#endif //INSTANCEOF_H
