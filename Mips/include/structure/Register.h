//
// Created by PengXinyang on 24-12-4.
//

#ifndef REGISTER_H
#define REGISTER_H
#include<map>
#include<string>
#include <unordered_map>
#include <vector>
using namespace std;
/**
* 寄存器枚举类
*/
enum class RegisterName {
    $zero,   // 0号寄存器
    $at,     // 汇编保留寄存器
    $v0, $v1, // 函数返回值寄存器
    $a0, $a1, $a2, $a3, // 函数参数寄存器
    $t0, $t1, $t2, $t3, $t4, $t5, $t6, $t7, // 临时寄存器
    $s0, $s1, $s2, $s3, $s4, $s5, $s6, $s7, // 保存寄存器
    $t8, $t9, // 额外的临时寄存器
    $k0, $k1, // 保留给操作系统使用
    $gp,      // 全局指针
    $sp,      // 堆栈指针
    $fp,      // 帧指针
    $ra       // 返回地址寄存器
};

// 加法操作符重载
inline RegisterName operator+(RegisterName reg, int value) {
    return static_cast<RegisterName>(static_cast<int>(reg) + value);
}

// 减法操作符重载
inline RegisterName operator-(RegisterName reg, int value) {
    return static_cast<RegisterName>(static_cast<int>(reg) - value);
}

class Register {
private:
    RegisterName reg = RegisterName::$zero;
    //采用类似于单例模式，保证每个寄存器类的指针也一样
    static unordered_map<RegisterName,Register*> registers;
    Register() = default;
    explicit Register(RegisterName reg);
public:
    [[nodiscard]] RegisterName getRegisterName() const;
    static Register* getRegister(RegisterName reg);
    string toMips();
    static RegisterName regTransform(int index) {
        return static_cast<RegisterName>(index);
    }
};

#endif //REGISTER_H
