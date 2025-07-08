//
// Created by PengXinyang on 24-9-20.
//

#ifndef TOKENTYPE_H
#define TOKENTYPE_H

#include<map>
#include <string>
using namespace std;

//单词类别
class TokenType {
public:
    //根据单词token获取对应的类型
    string getTokenType(const string& token);
    //获取单例模式TokenType
    static TokenType* getTokenTypeInstance();
    //进程退出时释放单实例
    static void deleteTokenTypeInstance();
    //获取map
    map<string, string> getTokenTypeMap();
protected:
    //构造函数
    TokenType();
private:
    //记录单词类型的map
    map<string, string> tokenTypeMap;
    // 唯一单实例对象指针
    static TokenType *g_pTokenType;
};

#endif //TOKENTYPE_H
